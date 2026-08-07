Option Explicit
Declare Function GetTempFileName Lib "Kernel" (ByVal cDriveLetter As Integer, ByVal lpPrefixString As String, ByVal wUnique As Integer, ByVal lpTempFileName As String) As Integer
Declare Function GetPrivateProfileString Lib "Kernel" (ByVal lpApplicationName As String, ByVal lpKeyName As String, ByVal lpDefault As String, ByVal lpReturnedString As String, ByVal nSize As Integer, ByVal lpFileName As String) As Integer

Global Const ATTR_DIRECTORY = 16
Global Const floppySize = 1454080
Global Const mySize = 30030
Global Const inst1Size = 11019
Global Const inst2Size = 83863

Global fentFeina As Integer
Global idtFile As String
Global BaseFolder As String
Global pacFolder As String
Global instFName As String
Global fileName() As String
Global fileSize() As Long
Global numFiles As Integer
Global totalSize As Long
Global title As String
Global authors As String
Global diskPath As String
Global promptForDisk As Integer
Global cancel As Integer
Global dlgResult As String
Global dlgParam As String
Global tempFile As String
Global srcFolder As String

Type idtData
  title As String
  authors As String
  pacFolder As String
  icon1file As String
  icon1desc As String
  icon1ico As String
  icon2file As String
  icon2desc As String
  icon2ico As String
End Type

Global idt As idtData

Sub Acaba ()

  If tempFile <> "" Then
    Kill tempFile
    tempFile = ""
  End If

End

End Sub

Function buildFileList () As Integer

Dim fn As String

buildFileList = False
totalSize = 0
numFiles = 0

ReDim fileName(10)
ReDim fileSize(10)

On Error GoTo errBFL
fn = "inst2.exe"
numFiles = numFiles + 1
fileName(numFiles) = fn
fileSize(numFiles) = inst2Size
totalSize = totalSize + fileSize(numFiles) + Len(fn) + 5

fn = "instal.idt"
numFiles = numFiles + 1
fileName(numFiles) = fn
fileSize(numFiles) = FileLen(idtFile)
totalSize = totalSize + fileSize(numFiles) + Len(fn) + 5

If recurseFolder(pacFolder) = False Then Exit Function

buildFileList = True
Exit Function

errBFL:
If MostraError(fn) = 2 Then Exit Function
Resume

End Function

Sub centrarImatge (f As Form)

Dim px As Integer, py As Integer

px = (screen.Width - f.Width) / 2
If px < 0 Then px = 0
py = (screen.Height - f.Height) / 2
If py < 0 Then py = 0
f.Left = px
f.Top = py

End Sub

Function createTempFile () As String

Dim r As Integer
Dim tmpf0 As String * 200
Dim tmpf As String
Dim i As Integer

On Error GoTo salta

createTempFile = ""
r = GetTempFileName(0, "inst", 0, tmpf0)
i = InStr(tmpf0, Chr(0))
If i < 2 Then
  MsgBox "ERROR: Unable to create a temporary file for the installation. Please check the TEMP system variable.", 16
  Exit Function
End If
tmpf = Left(tmpf0, i - 1)
createTempFile = tmpf

Exit Function

retorna:
Resume Next

salta:
If MostraError("") = 2 Then Exit Function
Resume

End Function

Function MostraError (fn As String) As Integer

Dim m As String

Select Case Err
  Case 7
    m = "Not enough memory"
  Case 53
    m = "Unable to find the file " + fn
  Case 57
    m = "Device error"
  Case 61:
    m = "Not enough free space in this disk!"
  Case 68, 75:
    m = "The destination unit is not accessible or is write-protected."
  Case 70
    m = "Unable to write in this diskette because it's write-protected!"
  Case 71
    m = "Unable to read from the source unit."
  Case 76:
    m = "Wrong folder name!"
  Case Else
    m = "ERROR " + Str(Err) + ":" + Chr(13) + Error
End Select

MostraError = MsgBox(m, 5 + 48, "ERROR")

End Function

Function recurseFolder (folderToSearch As String) As Integer

Dim localBaseFolder As String
Dim dirName As String
Dim count As Integer
Dim d() As String
Dim fn As String
Dim i As Integer

On Error GoTo errRF

recurseFolder = False
localBaseFolder = BaseFolder
If folderToSearch <> "" Then
  localBaseFolder = localBaseFolder & "\" & folderToSearch
End If

count = 0
dirName = Dir(localBaseFolder & "\*.*", ATTR_DIRECTORY)
Do While dirName <> ""
  If dirName <> "." And dirName <> ".." Then
    If (GetAttr(localBaseFolder & "\" & dirName) And ATTR_DIRECTORY) = ATTR_DIRECTORY Then
      If (count Mod 10) = 0 Then
	ReDim Preserve d(count + 10)
      End If
      count = count + 1
      d(count) = dirName
    End If
  End If
  dirName = Dir
  DoEvents
Loop

fn = Dir(localBaseFolder & "\*.*", 0)
Do While fn <> ""
  If (numFiles Mod 10) = 0 Then
    ReDim Preserve fileName(numFiles + 10)
    ReDim Preserve fileSize(numFiles + 10)
  End If
  numFiles = numFiles + 1
  fn = folderToSearch & "\" & fn
  fileName(numFiles) = fn
  fileSize(numFiles) = FileLen(BaseFolder & "\" & fn)
  totalSize = totalSize + fileSize(numFiles) + Len(fn) + 5
  fn = Dir
  DoEvents
Loop

For i = 1 To count
  If recurseFolder(d(i)) = False Then Exit Function
  DoEvents
Next i

recurseFolder = True
Exit Function

errRF:
If MostraError(fn) = 2 Then Exit Function
Resume

End Function

Function writeToPath (destPath As String) As Integer

Dim i As Integer
Dim b As String * 1
Dim s As String
Dim fn As String
Dim l As Long, l2 As Long
Dim disc As Integer
Dim ld As Long

b = Chr$(0)
ld = 0
disc = 1
writeToPath = False

If Right(destPath, 1) = "\" Then destPath = Left(destPath, Len(destPath) - 1)

If promptForDisk Then
  If totalSize < floppySize Then
    s = "Insert the 1,44 Mb disk in unit " & destPath
  Else
    s = "Insert the first disk in unit " & destPath
  End If
  If MsgBox(s, 1 + 64) = 2 Then
    MsgBox "Action cancelled!"
    Exit Function
  End If
End If

screen.MousePointer = 11
On Error GoTo errWTP
fn = destPath & "\" & instFName & ".exe"
Open fn For Binary Access Write As #1 Len = 12000

fn = app.Path
If Right(fn, 1) <> "\" Then fn = fn & "\"
fn = fn & app.EXEName & ".exe"
Open fn For Binary Access Read As #2 Len = 12000
  Seek #2, mySize + 1
  
  l = inst1Size
  ld = ld + l
  Do While l > 0
    If l > 12000 Then l2 = 12000 Else l2 = l
    s = Input(l2, #2)
    Put #1, , s
    l = l - l2
    DoEvents
  Loop

  For i = 1 To numFiles
    Put #1, , fileName(i)
    Put #1, , b
    Put #1, , fileSize(i)
    ld = ld + Len(fileName(i)) + 5
  Next i
  Put #1, , b
  ld = ld + 1

  l = inst2Size
  ld = ld + l
  Do While l > 0
    If l > 12000 Then l2 = 12000 Else l2 = l
    s = Input(l2, #2)
    Put #1, , s
    l = l - l2
    DoEvents
  Loop

Close #2


For i = 2 To numFiles
  If i = 2 Then
    fn = idtFile
  Else
    fn = BaseFolder + "\" + fileName(i)
  End If
  DoEvents
  Open fn For Binary Access Read As #2 Len = 12000
  l = fileSize(i)
  Do While l > 0
    If ld >= floppySize Then
      Close #1
      If promptForDisk Then
	screen.MousePointer = 0
	MsgBox "Please remove the disk from unit " & destPath
	disc = disc + 1
	If MsgBox("Insert the disk # " & disc & " in unit " & destPath, 1 + 64) = 2 Then
	  MsgBox "Action cancelled!"
	  Exit Function
	End If
	screen.MousePointer = 11
      Else
	disc = disc + 1
      End If
      fn = destPath & "\" & instFName & ".w" & Format(disc, "00")
      Open fn For Binary Access Write As #1 Len = 12000
      ld = 0
    End If
    If l > 12000 Then l2 = 12000 Else l2 = l
    If (ld + l2) > floppySize Then l2 = floppySize - ld
    s = Input(l2, #2)
    Put #1, , s
    l = l - l2
    ld = ld + l2
    DoEvents
  Loop
  Close #2
Next i

Close #1

screen.MousePointer = 0
If promptForDisk Then
  MsgBox "Please remove the disk from unit " & destPath
End If

writeToPath = True

Exit Function

errWTP:
screen.MousePointer = 0
If MostraError(fn) = 2 Then Exit Function
screen.MousePointer = 11
Resume

End Function

