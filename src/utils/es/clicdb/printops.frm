VERSION 2.00
Begin Form printops 
   BackColor       =   &H00C0C0C0&
   BorderStyle     =   3  'Fixed Double
   Caption         =   "Imprimir..."
   ClientHeight    =   2580
   ClientLeft      =   2085
   ClientTop       =   2205
   ClientWidth     =   4965
   ControlBox      =   0   'False
   Height          =   2985
   Icon            =   0
   Left            =   2025
   LinkTopic       =   "Form1"
   MaxButton       =   0   'False
   MinButton       =   0   'False
   ScaleHeight     =   2580
   ScaleWidth      =   4965
   Top             =   1860
   Width           =   5085
   Begin Frame sesOps 
      BackColor       =   &H00C0C0C0&
      Caption         =   "Opciones:"
      Height          =   855
      Left            =   420
      TabIndex        =   5
      Top             =   1080
      Width           =   4035
      Begin OptionButton opAll 
         BackColor       =   &H00C0C0C0&
         Caption         =   "De todas las sesiones"
         Height          =   255
         Left            =   240
         TabIndex        =   7
         Top             =   480
         Width           =   3735
      End
      Begin OptionButton opOne 
         BackColor       =   &H00C0C0C0&
         Caption         =   "Solo de la sesión seleccionada"
         Height          =   255
         Left            =   240
         TabIndex        =   6
         Top             =   240
         Width           =   3735
      End
   End
   Begin OptionButton opLlista 
      BackColor       =   &H00C0C0C0&
      Caption         =   "Imprimir una lista con los datos"
      Height          =   315
      Left            =   360
      TabIndex        =   4
      Top             =   660
      Width           =   4515
   End
   Begin OptionButton opScreen 
      BackColor       =   &H00C0C0C0&
      Caption         =   "Imprimir la pantalla tal como se ve"
      Height          =   315
      Left            =   360
      TabIndex        =   3
      Top             =   180
      Width           =   4575
   End
   Begin CommonDialog CMDialog1 
      Left            =   5160
      Top             =   120
   End
   Begin CommandButton BtnPrinter 
      Caption         =   "Impresora..."
      Height          =   375
      Left            =   3360
      TabIndex        =   2
      Top             =   2100
      Width           =   1455
   End
   Begin CommandButton BtnCancel 
      Cancel          =   -1  'True
      Caption         =   "Cancelar"
      Height          =   375
      Left            =   1740
      TabIndex        =   1
      Top             =   2100
      Width           =   1455
   End
   Begin CommandButton BtnAcceptar 
      Caption         =   "Imprimir"
      Default         =   -1  'True
      Height          =   375
      Left            =   120
      TabIndex        =   0
      Top             =   2100
      Width           =   1455
   End
End
Option Explicit

Sub BtnAcceptar_Click ()

poOk = True
Unload PrintOps

End Sub

Sub BtnCancel_Click ()

poOk = False
Unload PrintOps

End Sub

Sub BtnPrinter_Click ()

CMDialog1.PrinterDefault = True
CMDialog1.Action = 5

End Sub

Sub Form_Load ()

PrintOps.Icon = ClicDB.Icon
CentrarImatge PrintOps
If poScreen Then
  opScreen = True
  opOne.Enabled = False
  opAll.Enabled = False
Else
  opLlista = True
  opOne.Enabled = True
  opAll.Enabled = True
End If

If poAllSessions Then
  opAll = True
Else
  opOne = True
End If

sesOps.Visible = poShSess
opOne.Visible = poShSess
opAll.Visible = poShSess

poOk = False

End Sub

Sub opAll_Click ()

poAllSessions = True

End Sub

Sub opLlista_Click ()

poScreen = False
opOne.Enabled = True
opAll.Enabled = True

End Sub

Sub opOne_Click ()

poAllSessions = False

End Sub

Sub opScreen_Click ()

poScreen = True
opOne.Enabled = False
opAll.Enabled = False

End Sub

