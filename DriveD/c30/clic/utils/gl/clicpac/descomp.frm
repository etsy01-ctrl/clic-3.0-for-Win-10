VERSION 2.00
Begin Form Descomp 
   BackColor       =   &H00C0C0C0&
   BorderStyle     =   1  'Fixed Single
   Caption         =   "Descompactar"
   ClientHeight    =   3825
   ClientLeft      =   1320
   ClientTop       =   1830
   ClientWidth     =   5940
   ControlBox      =   0   'False
   Height          =   4230
   HelpContextID   =   200
   Icon            =   DESCOMP.FRX:0000
   Left            =   1260
   LinkTopic       =   "Form1"
   MaxButton       =   0   'False
   MinButton       =   0   'False
   ScaleHeight     =   3825
   ScaleWidth      =   5940
   Top             =   1485
   Width           =   6060
   Begin CommandButton BtnAjuda 
      BackColor       =   &H00C0C0C0&
      Caption         =   "Axuda"
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
   Begin CommandButton BtnDescomp 
      BackColor       =   &H00C0C0C0&
      Caption         =   "Extraer"
      Default         =   -1  'True
      Height          =   375
      Left            =   240
      TabIndex        =   2
      Top             =   3360
      Width           =   1455
   End
   Begin CommandButton BtnCancel 
      BackColor       =   &H00C0C0C0&
      Caption         =   "Cancelar"
      Height          =   375
      Left            =   2160
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
      Caption         =   "Arquivos a extraer:"
      Height          =   255
      Left            =   3240
      TabIndex        =   8
      Top             =   480
      Width           =   2415
   End
   Begin Label Label2 
      BackColor       =   &H00C0C0C0&
      Caption         =   "Seleccione os arquivos a extraer e a carpeta de destino:"
      Height          =   375
      Left            =   240
      TabIndex        =   7
      Top             =   120
      Width           =   5655
   End
   Begin Label Label1 
      BackColor       =   &H00C0C0C0&
      Caption         =   "Destino:"
      Height          =   255
      Left            =   240
      TabIndex        =   6
      Top             =   480
      Width           =   2535
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

Private Sub BtnDescomp_Click ()

  CANCEL = False
  CDLG.LB1 = "Descompactando..."
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

CDLG.LB1 = "Descompactando:"

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
  MsgBox "¡Non se seleccionou ningún arquivo para descompactar!"
Else
  MsgBox Str$(n) + " arquivos extraidos sen erros"
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
  MsgBox "¡Non se seleccionou ningún paquete para compactar!"
  Tanca
  Exit Sub
End If
If UCase$(Right$(a$, 3)) <> "PCC" Then
  MsgBox "Hai que seleccionar un paquete (arquivo *.PCC) para descompactar"
  Tanca
  Exit Sub
End If
If nfpcc < 1 Then
  MsgBox "¡Neste paquete non hai ningún arquivo para descompactar!"
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

