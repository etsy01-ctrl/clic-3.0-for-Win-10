VERSION 2.00
Begin Form Avis 
   BackColor       =   &H00C0C0C0&
   BorderStyle     =   1  'Fixed Single
   Caption         =   "DesInst"
   ClientHeight    =   1320
   ClientLeft      =   1320
   ClientTop       =   1770
   ClientWidth     =   4785
   ControlBox      =   0   'False
   Height          =   1725
   Icon            =   AVIS.FRX:0000
   Left            =   1260
   LinkTopic       =   "Form1"
   MaxButton       =   0   'False
   MinButton       =   0   'False
   ScaleHeight     =   1320
   ScaleWidth      =   4785
   Top             =   1425
   Width           =   4905
   Begin CommandButton Command1 
      Caption         =   "Cancel·lar"
      Height          =   375
      Left            =   1320
      TabIndex        =   0
      Top             =   720
      Width           =   2175
   End
   Begin Label Label1 
      Alignment       =   2  'Center
      BackStyle       =   0  'Transparent
      Caption         =   "Localitzant les aplicacions Clic instal·lades al vostre disc dur..."
      Height          =   375
      Left            =   240
      TabIndex        =   1
      Top             =   120
      Width           =   4335
   End
End
Option Explicit

Sub Command1_Click ()

  CANCEL = True
  Hide

End Sub

