VERSION 2.00
Begin Form sfolder 
   BackColor       =   &H00C0C0C0&
   BorderStyle     =   1  'Fixed Single
   Caption         =   "Cercar carpeta"
   ClientHeight    =   4665
   ClientLeft      =   3345
   ClientTop       =   3165
   ClientWidth     =   5415
   ControlBox      =   0   'False
   Height          =   5070
   Icon            =   SFOLDER.FRX:0000
   Left            =   3285
   LinkTopic       =   "Form2"
   MaxButton       =   0   'False
   MinButton       =   0   'False
   ScaleHeight     =   4665
   ScaleWidth      =   5415
   Top             =   2820
   Width           =   5535
   Begin CommandButton BtnCancel 
      Caption         =   "Cancel·lar"
      Height          =   435
      Left            =   2940
      TabIndex        =   8
      Top             =   4140
      Width           =   1875
   End
   Begin CommandButton BtnAcceptar 
      Caption         =   "Acceptar"
      Height          =   435
      Left            =   540
      TabIndex        =   7
      Top             =   4140
      Width           =   1875
   End
   Begin TextBox TxCarpeta 
      BackColor       =   &H00C0C0C0&
      BorderStyle     =   0  'None
      Enabled         =   0   'False
      Height          =   285
      Left            =   300
      TabIndex        =   6
      Top             =   3660
      Width           =   4875
   End
   Begin DirListBox Dir1 
      Height          =   1830
      Left            =   1200
      TabIndex        =   3
      Top             =   1740
      Width           =   3975
   End
   Begin DriveListBox Drive1 
      Height          =   315
      Left            =   1200
      TabIndex        =   0
      Top             =   1260
      Width           =   2655
   End
   Begin Label Label4 
      BackStyle       =   0  'Transparent
      Caption         =   "Nota: L'instal·lador copiarà també el contingut de les subcarpetes que pengin de la carpeta seleccionada."
      FontBold        =   0   'False
      FontItalic      =   0   'False
      FontName        =   "MS Sans Serif"
      FontSize        =   8.25
      FontStrikethru  =   0   'False
      FontUnderline   =   0   'False
      Height          =   555
      Left            =   180
      TabIndex        =   5
      Top             =   660
      Width           =   5055
   End
   Begin Label Label3 
      BackStyle       =   0  'Transparent
      Caption         =   "Seleccioneu la unitat i la carpeta on es troba el paquet d'activitats Clic."
      Height          =   495
      Left            =   180
      TabIndex        =   4
      Top             =   120
      Width           =   5175
   End
   Begin Label Label2 
      BackStyle       =   0  'Transparent
      Caption         =   "Carpeta:"
      Height          =   315
      Left            =   240
      TabIndex        =   2
      Top             =   1740
      Width           =   795
   End
   Begin Label Label1 
      BackStyle       =   0  'Transparent
      Caption         =   "Unitat:"
      Height          =   315
      Left            =   240
      TabIndex        =   1
      Top             =   1260
      Width           =   795
   End
End
Option Explicit

Sub BtnAcceptar_Click ()

If txCarpeta = "" Then
  MsgBox "Cal seleccionar una unitat i una carpeta!"
  Exit Sub
End If

dlgResult = txCarpeta
Unload SFolder

End Sub

Sub BtnCancel_Click ()

dlgResult = ""
cancel = True
Unload SFolder

End Sub

Sub Dir1_Change ()

txCarpeta = Dir1.Path

End Sub

Sub Drive1_Change ()

Dir1.Path = Drive1

End Sub

Sub Form_Load ()

centrarImatge SFolder
txCarpeta = Dir1.Path

End Sub

