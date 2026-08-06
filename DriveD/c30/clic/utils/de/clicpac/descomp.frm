VERSION 2.00
Begin Form Descomp 
   BackColor       =   &H00C0C0C0&
   BorderStyle     =   1  'Fixed Single
   Caption         =   "Dekomprimiert"
   ClientHeight    =   4215
   ClientLeft      =   945
   ClientTop       =   1560
   ClientWidth     =   5940
   ControlBox      =   0   'False
   Height          =   4620
   HelpContextID   =   200
   Icon            =   DESCOMP.FRX:0000
   Left            =   885
   LinkTopic       =   "Form1"
   MaxButton       =   0   'False
   MinButton       =   0   'False
   ScaleHeight     =   4215
   ScaleWidth      =   5940
   Top             =   1215
   Width           =   6060
   Begin CommandButton BtnAjuda 
      BackColor       =   &H00C0C0C0&
      Caption         =   "Hilfe"
      Height          =   375
      Left            =   4080
      TabIndex        =   9
      Top             =   3600
      Width           =   1575
   End
   Begin DirListBox Dir1 
      BackColor       =   &H00FFFFFF&
      Height          =   1830
      Left            =   240
      TabIndex        =   4
      Top             =   1320
      Width           =   2535
   End
   Begin DriveListBox Drive1 
      BackColor       =   &H00FFFFFF&
      Height          =   315
      Left            =   240
      TabIndex        =   3
      Top             =   960
      Width           =   2535
   End
   Begin CommandButton BtnDescomp 
      BackColor       =   &H00C0C0C0&
      Caption         =   "veschieben"
      Default         =   -1  'True
      Height          =   375
      Left            =   240
      TabIndex        =   2
      Top             =   3600
      Width           =   1455
   End
   Begin CommandButton BtnCancel 
      BackColor       =   &H00C0C0C0&
      Caption         =   "Abbrechen"
      Height          =   375
      Left            =   2160
      TabIndex        =   1
      Top             =   3600
      Width           =   1455
   End
   Begin ListBox DcList 
      BackColor       =   &H00FFFFFF&
      Height          =   2175
      Left            =   3240
      MultiSelect     =   2  'Extended
      TabIndex        =   0
      Top             =   960
      Width           =   2415
   End
   Begin Label Label3 
      BackColor       =   &H00C0C0C0&
      Caption         =   "Dateien zum Verschieben:"
      Height          =   255
      Left            =   3240
      TabIndex        =   8
      Top             =   720
      Width           =   2415
   End
   Begin Label Label2 
      BackColor       =   &H00C0C0C0&
      Caption         =   "Wähle die Dateien aus, welche verschoben werden sollen und den gewünschten Zielordner:"
      Height          =   375
      Left            =   240
      TabIndex        =   7
      Top             =   60
      Width           =   5655
   End
   Begin Label Label1 
      BackColor       =   &H00C0C0C0&
      Caption         =   "Ziel:"
      Height          =   255
      Left            =   240
      TabIndex        =   6
      Top             =   720
      Width           =   2535
   End
   Begin Label DestDir 
      BackColor       =   &H00C0C0C0&
      Caption         =   "dir"
      Height          =   255
      Left            =   240
      TabIndex        =   5
      Top             =   3240
      Width           =   5415
   End
End
Option Explicit

Sub BtnAjuda_Click ()

  SendKeys "{F1}"

End Sub

Private Sub BtnCancel_Click ()

CANCEL = True
Close
Tanca

End Sub

Private Sub BtnDescomp_Click ()

  CANCEL = False
  CDLG.LB1 = "dekomprimiert..."
  CDLG.LB2 = ""
  CDLG.Show
  popupOn = True
  DoEvents

  Descompacta
  
  popupOn = False
  CDLG.Hide
  Close

  Tanca

End Sub

Sub Descompacta ()

Dim fn$
Dim i As Integer, n As Integer

CDLG.LB1 = "dekomprimiert:"

DoEvents

fn$ = CompClic.LbFitx
On Error GoTo DetectaError
Open fn$ For Binary Access Read As #1 Len = 12000
If CANCEL Then Exit Sub
On Error GoTo 0

n = 0
For i = 1 To nfpcc
  If CANCEL Then Exit For
  If DCList.Selected(i - 1) = True Then
    fn$ = DestDir + "\" + PccDir(i).name
    If OverTest(fn$) Then
      CDLG.LB2 = fn$
      DoEvents
      DesCompFile i, fn$
      If CANCEL Then Exit Sub
      n = n + 1
    End If
  End If
Next i

Close

If n = 0 Then
  MsgBox "Sie haben keine Datei ausgewählt, die dekomprimiert werden soll!"
Else
  MsgBox Str$(n) + " verschobene Dateien ohne Fehler."
End If

Exit Sub
DetectaError:
MissatgeError fn$
Resume Next

End Sub

Private Sub Dir1_Change ()
  DestDir = Dir1
End Sub

Private Sub Drive1_Change ()
  Dir1.Path = Drive1
End Sub

Private Sub Form_Activate ()

  If popupOn Then
    CDLG.SetFocus
    Exit Sub
  End If

End Sub

Sub Form_Load ()

  Left = (screen.Width - Width) / 2
  Top = (screen.Height - Height) / 2
  Inicialitza

End Sub

Sub Inicialitza ()

Dim a$
Dim n As Integer, i As Integer

Drive1 = CompClic.Drive1
Dir1 = CompClic.Dir1
DestDir = Dir1
DCList.Clear

a$ = CompClic.LbFitx
If a$ = "" Then
  MsgBox "Sie haben keine Sammlung ausgewählt die komprimiert werden soll!"
  Tanca
  Exit Sub
End If
If UCase$(Right$(a$, 3)) <> "PCC" Then
  MsgBox "Sie müssen eine Sammlung zum Dekomprimieren auswählen (Datei *.PCC)"
  Tanca
  Exit Sub
End If
If nfpcc < 1 Then
  MsgBox "Diese Sammlung enthält keine Datei zum dekomprimieren!"
  Tanca
  Exit Sub
End If

For i = 1 To nfpcc
  DCList.AddItem PccDir(i).name
  DCList.Selected(i - 1) = True
Next i

End Sub

Sub Tanca ()

  Hide
  dscOn = False
  Close
  NetejaLlistaCompClic
  Unload Descomp

End Sub

