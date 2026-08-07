VERSION 2.00
Begin Form compact 
   BackColor       =   &H00C0C0C0&
   BorderStyle     =   3  'Fixed Double
   Caption         =   "Komprimierung der Datenbank"
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
      Caption         =   "Hilfe"
      Height          =   315
      Left            =   4500
      TabIndex        =   3
      Top             =   780
      Width           =   1575
   End
   Begin CommandButton BtnCancel 
      Caption         =   "Abbrechen"
      Height          =   315
      Left            =   2760
      TabIndex        =   2
      Top             =   780
      Width           =   1575
   End
   Begin CommandButton BtnCompacta 
      Caption         =   "Komprimierung beginnen"
      Height          =   315
      Left            =   120
      TabIndex        =   1
      Top             =   780
      Width           =   2475
   End
   Begin Label LbCompact 
      Alignment       =   2  'Center
      BackStyle       =   0  'Transparent
      Caption         =   "ACHTUNG: Dieser Vorgang dauert einige Minuten. Sind sie sicher, daﬂ Sie jetzt damit beginnen wollen?"
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

LbCompact = "Datenbank wurde kompriemiert..."
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
MsgBox "Die Komprimierung der Datenbank wurde mit Erfolg beendet.", 64
Unload Compact
Exit Sub

errCompact:
  screen.MousePointer = 0
  MsgBox "Es ist ein Fehler bei der Komprimierung der Daten aufgetreten!!!", 16, "FEHLER"
  Unload Compact
  Exit Sub

End Sub

Sub Form_Load ()

CentrarImatge Compact

End Sub

