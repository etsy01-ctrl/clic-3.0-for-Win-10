VERSION 2.00
Begin Form SFile 
   BackColor       =   &H00C0C0C0&
   BorderStyle     =   1  'Fixed Single
   Caption         =   "Cercar fitxer"
   ClientHeight    =   3810
   ClientLeft      =   3285
   ClientTop       =   3105
   ClientWidth     =   4275
   ControlBox      =   0   'False
   Height          =   4215
   Icon            =   SFILE.FRX:0000
   Left            =   3225
   MaxButton       =   0   'False
   MinButton       =   0   'False
   ScaleHeight     =   3810
   ScaleWidth      =   4275
   Top             =   2760
   Width           =   4395
   Begin CommandButton BtnCancel 
      Caption         =   "Cancel·lar"
      Height          =   435
      Left            =   2220
      TabIndex        =   2
      Top             =   3240
      Width           =   1875
   End
   Begin CommandButton BtnAcceptar 
      Caption         =   "Acceptar"
      Height          =   435
      Left            =   180
      TabIndex        =   1
      Top             =   3240
      Width           =   1875
   End
   Begin FileListBox File1 
      Height          =   2760
      Left            =   180
      TabIndex        =   0
      Top             =   360
      Width           =   3915
   End
   Begin Label Label3 
      BackStyle       =   0  'Transparent
      Caption         =   "Seleccioneu el fitxer en la llista:"
      Height          =   315
      Left            =   180
      TabIndex        =   3
      Top             =   60
      Width           =   5175
   End
End

Option Explicit

Sub BtnAcceptar_Click ()

If File1 = "" Then
  MsgBox "Cal seleccionar un fitxer!"
  Exit Sub
End If

dlgResult = File1
Unload SFile

End Sub

Sub BtnCancel_Click ()

dlgResult = ""
cancel = True
Unload SFile

End Sub

Sub Form_Load ()

  centrarImatge SFile
  File1.Path = dlgResult
  File1.Pattern = dlgParam


End Sub

