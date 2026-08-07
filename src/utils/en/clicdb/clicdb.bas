Option Explicit
Declare Function GetPrivateProfileString Lib "Kernel" (ByVal lpApplicationName As String, ByVal lpKeyName As String, ByVal lpDefault As String, ByVal lpReturnedString As String, ByVal nSize As Integer, ByVal lpFileName As String) As Integer
Declare Function GetProfileString Lib "Kernel" (ByVal lpApplicationName As String, ByVal lpKeyName As String, ByVal lpDefault As String, ByVal lpReturnedString As String, ByVal nSize As Integer) As Integer
Declare Function GetWindowsDirectory Lib "Kernel" (ByVal lpBuffer As String, ByVal nSize As Integer) As Integer

Global reportBase As String
Global tempDb As String
Global db As Database
Global tb As String

Global desDe As Variant
Global finsA As Variant
Global cancelData As Integer
Global dateFmt As String
' Efecte 2000
Global dateFmtLong As String

Global cancelEdit As Integer
Global editGroup As Integer
Global editItem As Long
Global editGroupId As Long

Global poOk As Integer
Global poScreen As Integer
Global poAllSessions As Integer
Global poShSess As Integer

Global marge As Integer
Global prnErr As String
Global maxY As Single
Global page As Integer
Global mp As String

Global lastName As String
Global directToUser As Long

Global Const DB_READONLY = 4

Sub CentrarImatge (f As Form)

If screen.Width <= 9600 And f.Width >= 9600 Then
  f.Left = 0
  f.Top = 0
Else
  f.Left = (screen.Width - f.Width) / 2
  f.Top = (screen.Height - f.Height) / 2
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

db.Close
End

End Sub

Function initGlobalVars () As Integer

   Dim errStr As String
   Dim l As String, s As String
   Dim k As Integer
   Dim iniFile As String
   Dim u As Dynaset
   Dim i As Integer

   initGlobalVars = False
   directToUser = 0
   lastName = ""
   poOk = False
   poScreen = True
   poAllSessions = False
   poShSess = False
   marge = 5
   mp = String(marge, " ")
   prnErr = "An error has been found trying to print the document"

   cancelEdit = False
   editGroup = False
   editItem = 0
   editGroupId = 0

   errStr = "ERROR: Not enough memory"
   On Error GoTo igbError
   
   l = String(200, Chr(0))
   k = GetProfileString("intl", "sShortDate", "mm/dd/yy", l, Len(l))
   dateFmt = Trim(Left(l, k))

   'Afegit per coregir efecte 2000
   If dateFmt = "" Then dateFmt = "mm/dd/yy"
   i = InStr(dateFmt, "y")
   If i < 1 Then
     dateFmtLong = "mm/dd/yyyy"
   Else
     s = ""
     If i > 1 Then s = Left(dateFmt, i - 1)
     s = s & "yyyy"
     For i = i + 1 To Len(dateFmt)
       If Mid(dateFmt, i, 1) <> "y" Then Exit For
     Next i
     If i <= Len(dateFmt) Then s = s & Mid(dateFmt, i)
     dateFmtLong = s
   End If
   'Fi de la correcció de l'efecte 2000

   reportBase = ""
   If Command <> "" Then
     i = 1
     l = UCase(Command)
     s = extractParam(l, i)
     While s <> ""
       If Left(s, 4) = "/DB:" Then
         reportBase = Mid(s, 5)
       ElseIf Left(s, 6) = "/USER:" Then
         directToUser = Val(Mid(s, 7))
       End If
       s = extractParam(l, i)
     Wend
   End If
   
   If reportBase = "" Then
     iniFile = app.Path & "\CLIC.INI"
     errStr = "ERROR: Cannot find CLIC.INI or has an incorrect format"
     l = String(200, Chr(0))
     k = GetPrivateProfileString("Clic", "reportBase", "", l, Len(l), iniFile)
     If k > 0 Then
       reportBase = Trim(Left(l, k))
     End If
   End If
   
   If reportBase = "" Then Error 32766
   
   errStr = "ERROR:Cannot open database " & reportBase
   Set db = OpenDatabase(reportBase, False, True)
   Set u = db.CreateDynaset("SELECT dateTime FROM sessions ORDER BY dateTime")
   If u.EOF Then
     desDe = CVDate(35796) 'aixo es 01/01/98
   Else
     u.MoveFirst
     desDe = CVDate(Format(u("dateTime"), dateFmtLong))
   End If
   u.Close
   finsA = CVDate(Format(Now, dateFmtLong))

   If desDe > finsA Then
     MsgBox "ATTENTION: the date on the first record on your database is later than the date in your computer. Check your system's date and time.", 48
     desDe = finsA
   End If

   For i = Len(reportBase) To 1 Step -1
     If Mid(reportBase, i, 1) = "\" Then Exit For
   Next i
   If i = 0 Then
     tempDb = Left(reportBase, Len(reportBase) - 5) & "k.mdb"
   Else
     tempDb = Left(reportBase, i) & "cmptmp.mdb"
   End If

   initGlobalVars = True
   Exit Function

igbError:
   MsgBox errStr, 16
   Exit Function

End Function

Function initPrinter () As Integer

Dim i As Integer

initPrinter = False

On Error GoTo errIniPrint

printer.FontName = "Courier New"
printer.FontSize = 9
For i = 1 To marge: printer.Print : Next i
maxY = printer.Height - printer.CurrentY
page = 1

initPrinter = True
Exit Function

errIniPrint:
MsgBox prnErr, 48
Exit Function

End Function

Sub NetejaPacs (sessionId As Long)

Dim p As Dynaset

Set p = db.CreateDynaset("SELECT * FROM Pacs WHERE sessionId=" & sessionId)
If Not p.EOF Then p.MoveFirst
Do Until p.EOF
  db.Execute "DELETE FROM Acts WHERE pacId=" & p("pacId")
  p.Delete
  p.MoveNext
Loop
p.Close

End Sub

Sub NetejaSessions (ByVal groupId As String, ByVal userId As String)

Dim q As querydef
Dim s As Dynaset

Set q = db.OpenQueryDef("selectSessions")
q!whatGroup = groupId
q!whatUser = userId
q!from = desDe
q!to = finsA + 1
Set s = q.CreateDynaset()
If Not s.EOF Then s.MoveFirst
Do Until s.EOF
  NetejaPacs Val(s("sessionId"))
  s.Delete
  s.MoveNext
Loop
s.Close

End Sub

Function printGrid (g As Grid, addColon As Integer, scanAll As Integer, titol As String, drawLine As Integer) As Integer

Dim ample() As Integer
Dim r As Integer, c As Integer, i As Integer
Dim s As String

On Error GoTo errPrintGrid
printGrid = False

ReDim ample(g.Cols)
For c = 0 To g.Cols - 1: ample(c) = 0: Next c

For r = 0 To g.Rows - 1
  g.Row = r
  For c = 0 To g.Cols - 1
    g.Col = c
    If Len(g.Text) > ample(c) Then ample(c) = Len(g.Text)
  Next c
  If scanAll = False And r = 1 Then Exit For
Next r

If addColon Then ample(0) = ample(0) + 1

For r = 0 To g.Rows - 1
  If printer.CurrentY >= maxY Then
    printer.Print
    printer.Print mp & "Page " & page
    page = page + 1
    printer.NewPage
    For i = 1 To marge: printer.Print : Next i
  End If
  
  If r = 0 Then
    printer.FontBold = True
    printer.Print mp & titol
    printer.FontBold = False
  End If
  
  g.Row = r
  printer.Print mp;
  For c = 0 To g.Cols - 1
    g.Col = c
    s = g.Text
    If c = 0 And addColon = True Then s = s & ":"
    i = False
    If c >= g.FixedCols Then
      If g.ColAlignment(c) <> 0 Then i = True
    End If
    If i Then
      s = String(ample(c) - Len(s), " ") & s
    Else
      s = s & String(ample(c) - Len(s), " ")
    End If
    printer.Print s & " ";
  Next c
  printer.Print

  If r = 0 And drawLine = True Then
    i = 0
    For c = 0 To g.Cols - 1: i = i + ample(c) + 1: Next c
    printer.Print mp & String(i, "-")
  End If

  DoEvents

Next r

printGrid = True
Exit Function

errPrintGrid:
MsgBox prnErr, 48
Exit Function

End Function

Function SecsToStr (ByVal secs As Long) As String

Dim s As String
Dim r As Long

s = ""

'If secs >= 60 Then
  If secs >= 3600 Then
    If secs >= 86400 Then
      r = secs Mod 86400
      s = s & (secs - r) / 86400 & ":"
      secs = r
    End If
    r = secs Mod 3600
    s = s & Format((secs - r) / 3600, "00:")
    secs = r
  End If
  r = secs Mod 60
  s = s & Format((secs - r) / 60, "00:")
  secs = r
'End If

s = s & Format(secs, "00")

SecsToStr = s

End Function

Sub SetGridText (g As Grid, x0 As Integer, y0 As Integer, x1 As Integer, y1 As Integer, s As String)

 g.SelEndRow = y1
 g.SelEndCol = x1
 g.SelStartRow = y0
 g.SelStartCol = x0
 g.Clip = s

End Sub

