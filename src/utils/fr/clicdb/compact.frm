VERSION 2.00
Begin Form compact 
   BackColor       =   &H00C0C0C0&
   BorderStyle     =   3  'Fixed Double
   Caption         =   "Compactage de la base de données"
   ClientHeight    =   1215
   ClientLeft      =   1995
   ClientTop       =   2925
   ClientWidth     =   6150
   ControlBox      =   0   'False
   Height          =   1620
   HelpContextID   =   800
   Icon            =   0
   Left            =   1935
   LinkTopic       =   "Form1"
   MaxButton       =   0   'False
   MinButton       =   0   'False
   ScaleHeight     =   1215
   ScaleWidth      =   6150
   Top             =   2580
   Width           =   6270
   Begin CommandButton BtnAjuda 
      Caption         =   "Aide"
      Height          =   315
      Left            =   4500
      TabIndex        =   3
      Top             =   780
      Width           =   1575
   End
   Begin CommandButton BtnCancel 
      Caption         =   "Annuler"
      Height          =   315
      Left            =   2760
      TabIndex        =   2
      Top             =   780
      Width           =   1575
   End
   Begin CommandButton BtnCompacta 
      Caption         =   "Commencer le compactage"
      Height          =   315
      Left            =   120
      TabIndex        =   1
      Top             =   780
      Width           =   2475
   End
   Begin Label LbCompact 
      Alignment       =   2  'Center
      BackStyle       =   0  'Transparent
      Caption         =   "ATTENTION: cette opération peut durer quelques minutes. Etes-vous sûr de vouloir la réaliser maintenant ?"
      Height          =   495
      Left            =   120
      TabIndex        =   0
      Top             =   120
      Width           =   5895
   End
End
Option Explicit

Sub BtnAjuda_Click ()

SendKeys "{F1}"

End Sub

Sub BtnCancel_Click ()

Unload Compact

End Sub

Sub BtnCompacta_Click ()

LbCompact = "Compactage de la base de données en cours..."
BtnCompacta.Enabled = False
BtnCancel.Enabled = False
Compact.Refresh
DoEvents

On Error GoTo errCompact
screen.MousePointer = 11
CompactDatabase reportBase, tempDB
Kill reportBase
Name tempDB As reportBase
screen.MousePointer = 0
MsgBox "Le compactage de la base de données s'est terminé avec succès.", 64
Unload Compact
Exit Sub

errCompact:
  screen.MousePointer = 0
  MsgBox "Une erreur grave s'est produite lors du compactage de la base de données!", 16, "ERREUR"
  Unload Compact
  Exit Sub

End Sub

Sub Form_Load ()

CentrarImatge Compact

End Sub

