VERSION 2.00
Begin Form Dates 
   BackColor       =   &H00C0C0C0&
   BorderStyle     =   3  'Fixed Double
   Caption         =   "Période des rapports"
   ClientHeight    =   1830
   ClientLeft      =   2625
   ClientTop       =   2640
   ClientWidth     =   3885
   ControlBox      =   0   'False
   Height          =   2235
   Icon            =   0
   Left            =   2565
   LinkTopic       =   "Form2"
   MaxButton       =   0   'False
   MinButton       =   0   'False
   ScaleHeight     =   1830
   ScaleWidth      =   3885
   Top             =   2295
   Width           =   4005
   Begin TextBox TxFinsA 
      Alignment       =   1  'Right Justify
      Height          =   285
      Left            =   1440
      TabIndex        =   5
      Top             =   720
      Width           =   1575
   End
   Begin TextBox TxDesDe 
      Alignment       =   1  'Right Justify
      Height          =   285
      Left            =   1440
      TabIndex        =   4
      Top             =   300
      Width           =   1575
   End
   Begin CommandButton BtnCancel 
      Caption         =   "Annuler"
      Height          =   375
      Left            =   2160
      TabIndex        =   1
      Top             =   1320
      Width           =   1215
   End
   Begin CommandButton BtnOk 
      Caption         =   "Accepter"
      Default         =   -1  'True
      Height          =   375
      Left            =   600
      TabIndex        =   0
      Top             =   1320
      Width           =   1215
   End
   Begin Label Label2 
      Alignment       =   1  'Right Justify
      BackColor       =   &H00C0C0C0&
      Caption         =   "Jusqu'à:"
      Height          =   255
      Left            =   60
      TabIndex        =   3
      Top             =   720
      Width           =   1335
   End
   Begin Label Label1 
      Alignment       =   1  'Right Justify
      BackColor       =   &H00C0C0C0&
      Caption         =   "De:"
      Height          =   255
      Left            =   60
      TabIndex        =   2
      Top             =   360
      Width           =   1335
   End
End
Option Explicit

Sub BtnCancel_Click ()

cancelData = True
Unload Dates

End Sub

Sub BtnOk_Click ()

Dim msg As String
Dim d1 As Variant, d2 As Variant

msg = "Erreur dans la date de début"
On Error GoTo ErrOk
d1 = CVDate(TxDesDe)

msg = "Erreur dans la date"
d2 = CVDate(TxFinsA)

If d2 < d1 Then
  MsgBox "La date de fin est antérieure à celle du début!", 48
  Exit Sub
End If

DesDe = d1
FinsA = d2
Unload Dates
Exit Sub

ErrOk:
MsgBox msg, 48
Exit Sub

End Sub

Sub Form_Load ()

Dates.icon = ClicDB.icon
CentrarImatge Dates
cancelData = False
TxDesDe = Format(DesDe, dateFmtLong)
TxFinsA = Format(FinsA, dateFmtLong)

End Sub

