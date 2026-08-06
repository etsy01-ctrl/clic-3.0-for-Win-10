VERSION 2.00
Begin Form CDLG 
   BackColor       =   &H00C0C0C0&
   BorderStyle     =   1  'Fixed Single
   Caption         =   "ClicPac"
   ClientHeight    =   1590
   ClientLeft      =   1305
   ClientTop       =   1740
   ClientWidth     =   4695
   ControlBox      =   0   'False
   Height          =   1995
   Icon            =   CDLG.FRX:0000
   Left            =   1245
   LinkTopic       =   "Form1"
   MaxButton       =   0   'False
   MinButton       =   0   'False
   ScaleHeight     =   1590
   ScaleWidth      =   4695
   Top             =   1395
   Width           =   4815
   Begin CommandButton CancBtn 
      Caption         =   "Cancel"
      Height          =   375
      Left            =   1200
      TabIndex        =   2
      Top             =   1080
      Width           =   2535
   End
   Begin Label LB2 
      Alignment       =   2  'Center
      BackStyle       =   0  'Transparent
      Caption         =   "-"
      FontBold        =   0   'False
      FontItalic      =   0   'False
      FontName        =   "MS Sans Serif"
      FontSize        =   8.25
      FontStrikethru  =   0   'False
      FontUnderline   =   0   'False
      Height          =   255
      Left            =   120
      TabIndex        =   1
      Top             =   600
      Width           =   4455
   End
   Begin Label LB1 
      Alignment       =   2  'Center
      BackStyle       =   0  'Transparent
      Caption         =   "-"
      Height          =   495
      Left            =   120
      TabIndex        =   0
      Top             =   120
      Width           =   4455
   End
End
Option Explicit

Sub CancBtn_Click ()

  CANCEL = True

End Sub

Sub Form_Load ()

  Left = (screen.Width - Width) / 2
  Top = (screen.Height - Height) / 2

End Sub

