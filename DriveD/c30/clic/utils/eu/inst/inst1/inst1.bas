Option Explicit
Declare Function GetTempFileName Lib "Kernel" (ByVal cDriveLetter As Integer, ByVal lpPrefixString As String, ByVal wUnique As Integer, ByVal lpTempFileName As String) As Integer
Declare Function GetModuleUsage Lib "Kernel" (ByVal hModule As Integer) As Integer

Global fentFeina As Integer
Global Const ATTR_DIRECTORY = 16
Global Const mySize = 11019
Global Const floppySize = 1454080

Sub centrarImatge (f As Form)

Dim px As Integer, py As Integer

px = (screen.Width - f.Width) / 2
If px < 0 Then px = 0
py = (screen.Height - f.Height) / 2
If py < 0 Then py = 0
f.Left = px
f.Top = py

End Sub

Function createTemp () As String

Dim r As Integer
Dim tmpf0 As String * 200
Dim tmpf As String
Dim i As Integer

On Error GoTo salta

createTemp = ""
r = GetTempFileName(0, "inst", 0, tmpf0)
i = InStr(tmpf0, Chr(0))
If i < 2 Then
  MsgBox "ERROREA: Ezin izan zen instalazioa egiteko karpeta iragankor bat sortu. Egiaztatu TEMP sistemaren aldagaiaren balioa", 16
  Exit Function
End If
tmpf = Left(tmpf0, i - 1)
On Error GoTo retorna
Kill tmpf
On Error GoTo salta

MkDir tmpf
createTemp = tmpf

Exit Function

retorna:
Resume Next

salta:
If mostraError("") = 2 Then Exit Function
Resume

End Function

Function cridaInstal (tempFolder As String) As Integer

Dim lRet As Long
Dim inst As Integer
Dim cmdLine As String

cmdLine = tempFolder & "\inst2.exe " & tempFolder & " instal.idt"

instal.Enabled = False
lRet = Shell(cmdLine)
inst = TestFunc(lRet)
While GetModuleUsage(inst) <> 0
  DoEvents
Wend
instal.Enabled = True
instal.SetFocus

cridaInstal = True

Exit Function

errCridaInstal:
If mostraError("") = 2 Then Exit Function
Resume

End Function

Function extractFiles (destPath As String) As Integer

Dim sourceFileBase As String
Dim s As String
Dim c As String * 1
Dim p As Long
Dim pf As Long
Dim btr As Long
Dim disc As Integer
Dim nFiles As Integer
Dim fName() As String
Dim fSize() As Long
Dim i As Integer
Dim destFile As String
Dim fn As String

On Error GoTo errExtract

extractFiles = False

disc = 1
sourceFileBase = app.Path
If Right(sourceFileBase, 1) <> "\" Then sourceFileBase = sourceFileBase & "\"
sourceFileBase = sourceFileBase & app.EXEName

fn = sourceFileBase & ".EXE"
Open fn For Binary Access Read Shared As #1

s = ""
p = mySize
Seek #1, p + 1
Do While Not EOF(1)
   Get #1, , c
   p = p + 1
   If c = Chr(0) Then Exit Do
   Do While Not EOF(1)
     s = s & c
     Get #1, , c
     p = p + 1
     If c = Chr(0) Then Exit Do
   Loop
   If nFiles Mod 10 = 0 Then
     ReDim Preserve fName(nFiles + 10)
     ReDim Preserve fSize(nFiles + 10)
   End If
   nFiles = nFiles + 1
   fName(nFiles) = s
   Get #1, , fSize(nFiles)
   p = p + 4
   s = ""
   DoEvents
Loop

For i = 1 To nFiles
  DoEvents
  destFile = destPath & "\" & fName(i)
  fn = destFile
  If testDir(destFile) = False Then Exit Function
  Open destFile For Binary Access Write As #2
  pf = fSize(i)
  Do While pf > 0
    If p >= floppySize Then
      disc = disc + 1
      Close #1
      fn = sourceFileBase & ".w" & Format(disc, "00")
      While Dir(fn, 0) = ""
	If MsgBox("Sar ezazu instalaziorako " & disc & "jarraitzeko", 64 + 1) = 2 Then
	  Close #2
	  Exit Function
	End If
      Wend
      Open fn For Binary Access Read Shared As #1
      p = 0
    End If
    btr = pf
    If btr > 12000 Then btr = 12000
    If p + btr > floppySize Then btr = floppySize - p
    s = Input(btr, #1)
    Put #2, , s
    p = p + btr
    pf = pf - btr
    DoEvents
  Loop
  Close #2
Next i

Close #1

extractFiles = True
Exit Function

errExtract:
If mostraError(fn) = 2 Then Exit Function
Resume

End Function

Sub killDir (dirToKill As String)

Dim count, d(), i, DirName
Dim fn As String

On Error GoTo errKillDir

fn = dirToKill
If Dir(dirToKill & "\*.*", 0) <> "" Then Kill dirToKill & "\*.*"

count = 0
DirName = Dir(dirToKill & "\*.*", ATTR_DIRECTORY)
Do While DirName <> ""
  DoEvents
  If DirName <> "." And DirName <> ".." Then
    fn = dirToKill & "\" & DirName
    If (GetAttr(fn) And ATTR_DIRECTORY) = ATTR_DIRECTORY Then
      If (count Mod 10) = 0 Then
	ReDim Preserve d(count + 10)
      End If
      count = count + 1
      d(count) = DirName
    End If
  End If
  DirName = Dir
Loop
    
For i = 1 To count
  fn = dirToKill & "\" & d(i)
  DoEvents
  killDir fn
Next i

fn = dirToKill
RmDir dirToKill

Exit Sub

errKillDir:
If mostraError(fn) = 2 Then Exit Sub
Resume

End Sub

Function mostraError (fn As String) As Integer

Dim m As String

Select Case Err
  Case 7
    m = "Memoria eznahikoa"
  Case 53
    m = "Ez da artxiboa aurkitzen " + fn
  Case 57
    m = "Dispositiboaren errorea"
  Case 61:
    m = "Diskete honetan ez dago espazio libre nahikoa"
  Case 68, 75:
    m = "Helmugako unitatea ez dago erabilgarri edo idazteko babestuta dago."
  Case 70
    m = "Ezin da idatzi unitate honetan!"
  Case 71
    m = "Ezin da irakurri iturburuko unitatetik!"
  Case 76:
    m = "Direktorioaren izena txarto dago"
  Case Else
    m = "ERROREA " + Str(Err) + ":" + Chr(13) + Error
End Select

mostraError = MsgBox(m, 5 + 48, "ERROREA")

End Function

Function testDir (fnt As String) As Integer

Dim i As Integer
Dim pt As String, a As String
Dim fn As String
Dim s As String

On Error GoTo errTestDir

testDir = False
fn = fnt
For i = Len(fnt) To 1 Step -1
  If Mid(fnt, i, 1) = "\" Then Exit For
Next i
pt = Left(fnt, i - 1)

fn = pt
On Error GoTo retornaTD
s = ""
s = Dir(pt, 16)
On Error GoTo errTestDir

If s = "" Then
  If testDir(pt) = False Then Exit Function
  MkDir pt
End If

testDir = True
Exit Function

retornaTD:
Resume Next

errTestDir:
If mostraError(fn) = 2 Then Exit Function
Resume

End Function

Function TestFunc (ByVal lVal As Long) As Integer
   'this function is necessary since the value returned by Shell is an
   'unsigned integer and may exceed the limits of a VB integer
   If (lVal And &H8000&) = 0 Then
     TestFunc = lVal And &HFFFF&
   Else
     TestFunc = &H8000 Or (lVal And &H7FFF&)
   End If
End Function

