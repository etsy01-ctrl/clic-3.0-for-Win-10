VERSION 2.00
Begin Form Instal 
   BackColor       =   &H00FF0000&
   BorderStyle     =   3  'Fixed Double
   Caption         =   "Installation of Clic activities"
   ClientHeight    =   1080
   ClientLeft      =   2955
   ClientTop       =   3420
   ClientWidth     =   7365
   ControlBox      =   0   'False
   Height          =   1485
   Icon            =   INSTAL.FRX:0000
   Left            =   2895
   LinkTopic       =   "Form1"
   MaxButton       =   0   'False
   MinButton       =   0   'False
   ScaleHeight     =   1080
   ScaleWidth      =   7365
   Top             =   3075
   Width           =   7485
   Begin Label Msg 
      Alignment       =   2  'Center
      BackStyle       =   0  'Transparent
      Caption         =   "Decompacting installation files..."
      FontBold        =   -1  'True
      FontItalic      =   0   'False
      FontName        =   "MS Sans Serif"
      FontSize        =   9.75
      FontStrikethru  =   0   'False
      FontUnderline   =   0   'False
      ForeColor       =   &H00FFFFFF&
      Height          =   375
      Left            =   0
      TabIndex        =   0
      Top             =   360
      Width           =   7335
   End
End
Option Explicit

Sub Form_Activate ()

Dim tempDir As String

If FentFeina <> 0 Then Exit Sub
FentFeina = True

screen.mousePointer = 11
tempDir = createTemp()
If tempDir = "" Then End

If extractFiles(tempDir) Then
  msg = "Installer running..."
  screen.mousePointer = 0
  If cridaInstal(tempDir) = False Then
    MsgBox "Unable to complete the installation!", 16
  End If
End If

screen.mousePointer = 11
msg = "Deleting temporary files..."
killDir tempDir
screen.mousePointer = 0

End

End Sub

Sub Form_Load ()

centrarImatge Instal
FentFeina = 0

End Sub

