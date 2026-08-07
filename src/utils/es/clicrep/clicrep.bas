Option Explicit

Global db As Database
Global a As dynaset
Global dbOpened As Integer
Global reportBase As String
Global currentGroup As Long
Global currentUser As Long
Global currentSession As Long
Global currentPac As Long
Global pacName As String
Global currentGroupName As String
Global currentUserName As String
Global newUsersAllowed As Integer
Global newGroupsAllowed As Integer

Global Const DB_READONLY = 4
Global Const DB_APPENDONLY = 4
Global Const MAXTRIES = 1000

Global Const FORE1 = 0
Global Const FORE2 = &HFF

Sub addAct (msg As String)

  Dim i As Integer
  Dim c As Integer

  c = 0
  If currentPac = 0 Then createPac
  If currentPac = 0 Then Exit Sub

  On Error GoTo errAddAct
  a.AddNew
  a("pacId") = currentPac
  i = 1
  a("act") = extractParam(msg, i)
  a("time") = Val(extractParam(msg, i))
  a("tries") = Val(extractParam(msg, i))
  a("placed") = Val(extractParam(msg, i))
  a("solved") = (extractParam(msg, i) = "X")
  a("precision") = Val(extractParam(msg, i))
  a.Update

Exit Sub

errAddAct:
c = c + 1
If c >= MAXTRIES Then
  MsgBox Error(Err), 16
  Exit Sub
Else
  DoEvents
  Resume
End If

End Sub

Sub AddSession ()

  Dim s As dynaset
  Dim c As Integer
  
  c = 0
  If currentUser = 0 Then Exit Sub

  Set s = db.CreateDynaset("sessions")
  On Error GoTo errAddSession
  s.AddNew
  s("userId") = currentUser
  s("dateTime") = Now
  s.Update
  s.FindLast "userId=" & Trim(Str(currentUser))
  currentSession = s("sessionId")
  On Error GoTo 0
  s.Close

Exit Sub

errAddSession:
c = c + 1
If c >= MAXTRIES Then
  MsgBox Error(Err), 16
  Exit Sub
Else
  DoEvents
  Resume
End If

End Sub

Sub createPac ()

  Dim p As dynaset
  Dim c As Integer

  c = 0
  If currentSession = 0 Then
    AddSession
    If currentSession = 0 Then Exit Sub
  End If

  Set p = db.CreateDynaset("pacs")
  On Error GoTo errAddPac
  p.AddNew
  p("pacName") = pacName
  p("sessionId") = currentSession
  p.Update
  p.FindLast "sessionId=" & Trim(Str(currentSession))
  currentPac = p("pacId")
  p.Close

Exit Sub

errAddPac:
c = c + 1
If c >= MAXTRIES Then
  MsgBox Error(Err), 16
  Exit Sub
Else
  DoEvents
  Resume
End If

End Sub

Function extractParam (s As String, p As Integer) As String

  Dim l As Integer
  Dim i As Integer, j As Integer
  
  extractParam = ""
  l = Len(s)
  For i = p To l
    If Mid(s, p, 1) <> " " Then Exit For
    p = p + 1
  Next i
  If i > l Then Exit Function

  j = p
  p = p + 1

  For i = p To l
    If Mid(s, p, 1) = " " Then Exit For
    p = p + 1
  Next i

  If p > j Then extractParam = Mid(s, j, p - j)

End Function

Sub Final ()

If dbOpened Then
  a.Close
  db.Close
  dbOpened = False
End If

End Sub

Function initGlobalVars () As Integer

   Dim errStr As String
   Dim p As Integer
   Dim i As Integer
   Dim c As String
   Dim iniFile As String
   Dim u As dynaset

   dbOpened = False
   currentGroup = 0
   currentUser = 0
   currentSession = 0
   currentPac = 0
   currentGroupName = ""
   currentUserName = ""
   newUsersAllowed = True
   newGroupsAllowed = True

   initGlobalVars = False
   If Command = "" Then Exit Function

   errStr = "ERROR: Memoria insuficiente"
   On Error GoTo igbError
   
   reportBase = ""
   pacName = "-"
   
   p = 1
   Do
     c = Trim(extractParam(Command, p))
     If c = "" Then Exit Do
     Select Case UCase(c)
       Case "NONEWUSR"
            newUsersAllowed = False

       Case "NONEWGRP"
            newGroupsAllowed = False
     
       Case Else
            i = InStr(c, ":")
            If i < 2 Then Exit Do
            Select Case UCase(Left(c, i - 1))
              Case "USER"
                   currentUser = Val(Mid(c, i + 1))

              Case "DB"
                   reportBase = Mid(c, i + 1)

            End Select
       End Select
   Loop

   If reportBase = "" Then Error 32766

   If openDb() = False Then Exit Function

   If currentUser <> 0 Then
     errStr = "ERROR: El usuario indicado no existe en la base de datos"
     Set u = db.CreateDynaset("users", DB_READONLY)
     u.FindFirst "userId=" & Trim(Str(currentUser))
     If u.NoMatch Then
       u.Close
       Error 32766
     End If
     currentGroup = u("groupId")
     u.Close
   End If

   initGlobalVars = True
   Exit Function

igbError:
   MsgBox errStr, 16
   Exit Function

End Function

Function openDb () As Integer

Dim errStr As String

openDb = False

Screen.MousePointer = 11
If dbOpened Then
  a.Close
  db.Close
End If

On Error GoTo odbError
errStr = "ERROR: Imposible abrir la base de datos " & reportBase
Set db = OpenDatabase(reportBase, False, False)
Set a = db.CreateDynaset("acts")
openDb = True
dbOpened = True
Screen.MousePointer = 0

Exit Function

odbError:
Screen.MousePointer = 0
MsgBox errStr, 16
Exit Function

End Function

