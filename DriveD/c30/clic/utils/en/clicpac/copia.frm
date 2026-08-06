VERSION 2.00
Begin Form Copia 
   BackColor       =   &H00C0C0C0&
   BorderStyle     =   1  'Fixed Single
   Caption         =   "Copy"
   ClientHeight    =   3825
   ClientLeft      =   495
   ClientTop       =   690
   ClientWidth     =   5940
   ControlBox      =   0   'False
   Height          =   4230
   HelpContextID   =   300
   Left            =   435
   LinkTopic       =   "Form1"
   MaxButton       =   0   'False
   MinButton       =   0   'False
   ScaleHeight     =   3825
   ScaleWidth      =   5940
   Top             =   345
   Width           =   6060
   Begin CommandButton BtnAjuda 
      BackColor       =   &H00C0C0C0&
      Caption         =   "Help"
      Height          =   375
      Left            =   4080
      TabIndex        =   9
      Top             =   3360
      Width           =   1575
   End
   Begin DirListBox Dir1 
      BackColor       =   &H00FFFFFF&
      Height          =   1830
      Left            =   240
      TabIndex        =   4
      Top             =   1080
      Width           =   2535
   End
   Begin DriveListBox Drive1 
      BackColor       =   &H00FFFFFF&
      Height          =   315
      Left            =   240
      TabIndex        =   3
      Top             =   720
      Width           =   2535
   End
   Begin CommandButton BtnCopiar 
      BackColor       =   &H00C0C0C0&
      Caption         =   "Copy"
      Default         =   -1  'True
      Height          =   375
      Left            =   240
      TabIndex        =   2
      Top             =   3360
      Width           =   1455
   End
   Begin CommandButton BtnCancel 
      BackColor       =   &H00C0C0C0&
      Caption         =   "Cancel"
      Height          =   375
      Left            =   2280
      TabIndex        =   1
      Top             =   3360
      Width           =   1455
   End
   Begin ListBox DcList 
      BackColor       =   &H00FFFFFF&
      Height          =   2175
      Left            =   3240
      MultiSelect     =   2  'Extended
      TabIndex        =   0
      Top             =   720
      Width           =   2415
   End
   Begin Label Label3 
      BackColor       =   &H00C0C0C0&
      Caption         =   "Files to copy:"
      Height          =   255
      Left            =   3240
      TabIndex        =   8
      Top             =   480
      Width           =   2355
   End
   Begin Label Label2 
      BackColor       =   &H00C0C0C0&
      Caption         =   "Select files to copy and destination folder:"
      Height          =   375
      Left            =   240
      TabIndex        =   7
      Top             =   120
      Width           =   5655
   End
   Begin Label Label1 
      BackColor       =   &H00C0C0C0&
      Caption         =   "Destination"
      Height          =   255
      Left            =   240
      TabIndex        =   6
      Top             =   480
      Width           =   2475
   End
   Begin Label DestDir 
      BackColor       =   &H00C0C0C0&
      Caption         =   "dir"
      Height          =   255
      Left            =   240
      TabIndex        =   5
      Top             =   3000
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

Private Sub BtnCopiar_Click ()

  If DestDir = CompClic.Dir1.Path Then
    MsgBox "Must select a different directory to copy the files!!", 16, "ERROR"
    Exit Sub
  End If
  
  
  CANCEL = False
  CDLG.LB1 = "Copying..."
  CDLG.LB2 = ""
  CDLG.Show
  popupOn = True
  DoEvents

  FesCopia
  
  popupOn = False
  CDLG.Hide
  Close

  Tanca

End Sub

Private Sub Dir1_Change ()
  DestDir = Dir1
End Sub

Private Sub Drive1_Change ()
  Dir1.Path = Drive1
End Sub

Sub FesCopia ()

Dim fn$
Dim i As Integer, n As Integer
Dim l As Long, l2 As Long
Dim s As String

CDLG.LB1 = "Copying:"
  
DoEvents

n = 0
For i = 1 To DcList.ListCount
  If CANCEL Then Exit For
  If DcList.Selected(i - 1) = True Then
    n = n + 1

    fn$ = DestDir + "\" + DcList.List(i - 1)
    If OverTest(fn$) Then
      CDLG.LB2 = fn$
      DoEvents
      
      fn$ = CompClic.Dir1.Path + "\" + DcList.List(i - 1)
      On Error GoTo DetectaError
      Open fn$ For Binary Access Read As #1 Len = 12000
      If CANCEL Then Exit Sub
      On Error GoTo 0
      
      l = LOF(1)
      If l > 0 Then
        fn$ = DestDir + "\" + DcList.List(i - 1)
        On Error GoTo DetectaError
        Open fn$ For Binary Access Write As #2 Len = 12000
        If CANCEL Then Exit Sub
        On Error GoTo 0
      
        Do While l > 0 And CANCEL = False
          If l > 12000 Then l2 = 12000 Else l2 = l
          s = Input(l2, #1)
          Put #2, , s
          l = l - l2
          DoEvents
        Loop
      Else
        MsgBox "ATTENTION: File " & fn$ & " does not exist!"
      End If

      Close #2
      Close #1
    End If
  End If
Next i

Close

If n = 0 Then
  MsgBox "No file selected to copy!"
Else
  MsgBox Str$(n) + " files copied without errors"
End If

Exit Sub
DetectaError:
MissatgeError fn$
Resume Next

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
DcList.Clear

a$ = CompClic.LbFitx
If a$ = "" Then
  MsgBox "NO pack selected to copy!"
  Tanca
  Exit Sub
End If
If UCase$(Right$(a$, 3)) <> "PAC" Then
  MsgBox "Must select a pack (*.PAC file) to copy"
  Tanca
  Exit Sub
End If
If CompClic.Lst.ListCount < 1 Then
  MsgBox "No file to copy in this pack"
  Tanca
  Exit Sub
End If

For i = 1 To CompClic.Lst.ListCount
  DcList.AddItem CompClic.Lst.List(i - 1)
  DcList.Selected(i - 1) = True
Next i

End Sub

Sub Tanca ()

  Hide
  copyOn = False
  Close
  NetejaLlistaCompClic
  Unload Copia

End Sub

