VERSION 2.00
Begin Form compact 
   BackColor       =   &H00C0C0C0&
   BorderStyle     =   3  'Fixed Double
   Caption         =   "Compactació de la base de dades"
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
      Caption         =   "Ajuda"
      Height          =   315
      Left            =   4500
      TabIndex        =   3
      Top             =   780
      Width           =   1575
   End
   Begin CommandButton BtnCancel 
      Caption         =   "Cancel·lar"
      Height          =   315
      Left            =   2760
      TabIndex        =   2
      Top             =   780
      Width           =   1575
   End
   Begin CommandButton BtnCompacta 
      Caption         =   "Iniciar la compactació"
      Height          =   315
      Left            =   120
      TabIndex        =   1
      Top             =   780
      Width           =   2475
   End
   Begin Label LbCompact 
      Alignment       =   2  'Center
      BackStyle       =   0  'Transparent
      Caption         =   "ATENCIÓ: Aquesta operació pot trigar una estona llarga a completar-se. Segur que voleu fer-la ara?"
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

LbCompact = "Compactant la base de dades..."
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
MsgBox "L'operació de compactar la base de dades s'ha completat amb èxit", 64
Unload Compact
Exit Sub

errCompact:
  screen.MousePointer = 0
  MsgBox "S'ha produït un error fatal mentre es compactava la base de dades!!!", 16, "ERROR"
  Unload Compact
  Exit Sub

End Sub

Sub Form_Load ()

CentrarImatge Compact

End Sub

