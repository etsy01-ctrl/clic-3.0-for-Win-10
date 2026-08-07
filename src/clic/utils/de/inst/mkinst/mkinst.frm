VERSION 2.00
Begin Form MkInst 
   BackColor       =   &H00FF0000&
   BorderStyle     =   3  'Fixed Double
   Caption         =   "MkInst 1.0"
   ClientHeight    =   2925
   ClientLeft      =   1680
   ClientTop       =   2430
   ClientWidth     =   7395
   ControlBox      =   0   'False
   Height          =   3330
   Icon            =   MKINST.FRX:0000
   Left            =   1620
   LinkTopic       =   "Form1"
   MaxButton       =   0   'False
   MinButton       =   0   'False
   ScaleHeight     =   2925
   ScaleWidth      =   7395
   Top             =   2085
   Width           =   7515
   Begin CommandButton BtnCancel 
      Caption         =   "Abbrechen"
      Height          =   375
      Left            =   4440
      TabIndex        =   3
      Top             =   2280
      Width           =   2175
   End
   Begin CommandButton BtnCrea 
      Caption         =   "Erstellen der Installationsdisketten"
      Height          =   375
      Left            =   480
      TabIndex        =   2
      Top             =   2280
      Width           =   3495
   End
   Begin Image Image1 
      Height          =   480
      Left            =   120
      Picture         =   MKINST.FRX:0302
      Top             =   120
      Width           =   480
   End
   Begin Label msg2 
      Alignment       =   2  'Center
      BackStyle       =   0  'Transparent
      Caption         =   "Initialisieren..."
      FontBold        =   -1  'True
      FontItalic      =   0   'False
      FontName        =   "MS Sans Serif"
      FontSize        =   9.75
      FontStrikethru  =   0   'False
      FontUnderline   =   0   'False
      ForeColor       =   &H00FFFFFF&
      Height          =   735
      Left            =   240
      TabIndex        =   1
      Top             =   1080
      Width           =   6855
   End
   Begin Label Msg 
      Alignment       =   2  'Center
      BackStyle       =   0  'Transparent
      Caption         =   "Erstellen der Installationsdisketten der Clic Übungen"
      FontBold        =   -1  'True
      FontItalic      =   0   'False
      FontName        =   "MS Sans Serif"
      FontSize        =   9.75
      FontStrikethru  =   0   'False
      FontUnderline   =   0   'False
      ForeColor       =   &H00FFFFFF&
      Height          =   855
      Left            =   720
      TabIndex        =   0
      Top             =   120
      Width           =   6375
   End
End
Option Explicit

Sub BtnCancel_Click ()

Acaba

End Sub

Sub BtnCrea_Click ()

Dim s As String

If tempFile <> "" Then
  SDest.Show 1
  If Cancel Then Exit Sub
  diskPath = dlgResult
  Select Case UCase(Left(diskPath, 1))
    Case "bis", "B"
      promptForDisk = True
    Case Else
      promptForDisk = False
  End Select
End If

msg2 = "Erstellen des Installers..."
DoEvents

If writeToPath(diskPath) = False Then Acaba
If Not promptForDisk Then
  MsgBox "Installationsdateien erfolgreich erstellt in " & diskPath
End If
Acaba

End Sub

Sub Form_Activate ()

Dim s As String
Dim nDisks As Integer
Dim ts As Long

If fentFeina <> 0 Then Exit Sub
fentFeina = 1

msg2 = "Die Größe der Dateien kalkulieren..."
DoEvents

screen.MousePointer = 11
If buildFileList() = False Then Acaba
screen.MousePointer = 0

nDisks = 0
ts = totalSize
Do While ts > 0
  nDisks = nDisks + 1
  ts = ts - floppySize
Loop

If nDisks = 1 Then
  ts = totalSize / 1024
  If ts < 1 Then ts = 1
  msg2 = "Um Installers dieser Übungssammlung zu erstellen brauchen Sie eine formatierte 1,44 Mb Diskette " & ts & " Kb"
Else
  msg2 = "Um Installers von dieser Übungssammlung zu erstellen, brauchen Sie " & nDisks & " formatierte und leere Disketten"
End If

btnCrea.Enabled = True

End Sub

Sub Form_Load ()

Dim p As Integer, j As Integer
Dim l As String
Dim k As Integer

Cancel = False
diskPath = "A:"
promptForDisk = True
centrarImatge mkinst
tempFile = ""
DoEvents

If Command = "" Then
  tempFile = createTempFile()
  If tempFile = "" Then End
  Load IDTParam
  mkinst.Hide
  IDTParam.Show 1
  If Cancel Then Acaba
  mkinst.Show
  baseFolder = dlgResult
  idtFile = tempFile

Else

  j = InStr(app.Path, "\")
  p = InStr(Command, " ")
  If p > 0 Then
    baseFolder = Trim(Left(Command, p))
    If InStr(baseFolder, ":") = 0 Then
      baseFolder = Left(app.Path, j) & baseFolder
    End If
    If Right(baseFolder, 1) = "\" Then baseFolder = Left(baseFolder, Len(baseFolder) - 1)
    idtFile = Trim(Mid(Command, p + 1))
    If InStr(idtFile, ":") = 0 Then
      idtFile = Left(app.Path, j) & idtFile
    End If
  End If

  If (baseFolder = "") Or (idtFile = "") Then
    MsgBox "Syntaxerror!"
    Acaba
  End If
End If

l = String(200, Chr(0))
k = GetPrivateProfileString("Instal", "folder", "", l, Len(l), idtFile)
If k > 0 Then
  pacFolder = Trim(Left(l, k))
End If

If (pacFolder = "") Then
  MsgBox "ERROR: fehlerhafte IDT Datei."
  Acaba
End If

l = String(200, Chr(0))
k = GetPrivateProfileString("Instal", "title", "", l, Len(l), idtFile)
If k > 0 Then
  title = Trim(Left(l, k))
End If

If (title = "") Then
  MsgBox "ERROR: fehlerhafte IDT Datei."
  Acaba
End If

l = String(200, Chr(0))
k = GetPrivateProfileString("Instal", "authors", "", l, Len(l), idtFile)
If k > 0 Then
  authors = Trim(Left(l, k))
End If

l = "Erstellen der Installationsdisketten für Clic Übungen " & Chr(34) & title & Chr(34)
If authors <> "" Then l = l & " von " & authors

msg = l

For p = Len(pacFolder) To 1 Step -1
  If Mid(pacFolder, p, 1) = "\" Then Exit For
Next p

If p = 0 Then
  instFName = pacFolder
Else
  instFName = Mid(pacFolder, p + 1)
End If

If Len(instFName) > 8 Then instFName = Left(instFName, 8)

fentFeina = 0
btnCrea.Enabled = False

End Sub

