VERSION 2.00
Begin Form ClicRep 
   BorderStyle     =   0  'None
   Caption         =   "ClicRep"
   ClientHeight    =   1485
   ClientLeft      =   570
   ClientTop       =   7500
   ClientWidth     =   2490
   ControlBox      =   0   'False
   Enabled         =   0   'False
   Height          =   1890
   Icon            =   CLICREP.FRX:0000
   Left            =   510
   LinkMode        =   1  'Source
   LinkTopic       =   "infoclic"
   MaxButton       =   0   'False
   MinButton       =   0   'False
   ScaleHeight     =   1485
   ScaleWidth      =   2490
   Top             =   7155
   Visible         =   0   'False
   Width           =   2610
   WindowState     =   1  'Minimized
End
Option Explicit

Sub activaClicDB ()

 Dim s As String
 Dim x As Integer

 If currentUser = 0 Then
   MsgBox "ERROR: There is no user identified in the report system", 16, "ERROR"
   Exit Sub
 End If
 
 On Error GoTo errClicDB
 s = app.Path & "\CLICDB.EXE /DB:" & reportBase & " /USER:" & Trim(Str(currentUser))
 x = Shell(s)

Exit Sub

errClicDB:
 MsgBox "An error has been made trying to activate the report screen. Make sure CLICDB.EXE is in Clic folder.", 16, "ERROR"
 Exit Sub

End Sub

Sub CaptaMissatge (msg As String)
  
  Dim i As Integer
  
  If msg = "" Then Exit Sub
  
  DoEvents

  Select Case msg
    Case "CLICDB"
      activaClicDB
    
    Case "END"
      Final
    
    Case "BEGIN_REPORT"
      currentSession = 0
      currentPac = 0
      pacName = "-"
      If currentUser = 0 Then ident.Show 1

    Case "END_REPORT"
      currentGroup = 0
      currentUser = 0
      currentSession = 0
      currentPac = 0
      pacName = "-"
    
    Case Else
      If UCase(Left(msg, 6)) = "SETDB:" Then
        currentGroup = 0
        currentUser = 0
        currentSession = 0
        currentPac = 0
        pacName = "-"
        reportBase = Mid(msg, 7)
        If openDb() = False Then Final
      Else
        If currentUser = 0 Then Exit Sub
        i = InStr(msg, ".")
        If i <> 0 And Len(msg) >= i + 3 Then
          Select Case UCase(Mid(msg, i + 1, 3))
            Case "PAC", "PCC"
              pacName = msg
              currentPac = 0
            Case Else
              Screen.MousePointer = 11
              addAct msg
              Screen.MousePointer = 0
          End Select
        End If
      End If
  End Select
  
End Sub

Sub Form_LinkClose ()

Final
End

End Sub

Sub Form_LinkError (LinkErr As Integer)

MsgBox "Link error: " & LinkErr

End Sub

Sub Form_LinkExecute (CmdStr As String, Cancel As Integer)

CaptaMissatge CmdStr
Cancel = 0

End Sub

Sub Form_Load ()


app.HelpFile = app.Path & "\clic.hlp"
If app.PrevInstance Then End
If initGlobalVars() = False Then End

'amagar això
'ident.Show 1

End Sub

Sub Form_Unload (Cancel As Integer)

Final

End Sub

