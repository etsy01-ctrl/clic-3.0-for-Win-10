VERSION 2.00
Begin Form ClicDB 
   BackColor       =   &H00C0C0C0&
   BorderStyle     =   1  'Fixed Single
   Caption         =   "ClicDB"
   ClientHeight    =   3750
   ClientLeft      =   1635
   ClientTop       =   1980
   ClientWidth     =   6915
   Height          =   4155
   Icon            =   CLICDB.FRX:0000
   Left            =   1575
   LinkTopic       =   "Form1"
   MaxButton       =   0   'False
   ScaleHeight     =   3750
   ScaleWidth      =   6915
   Top             =   1635
   Width           =   7035
   Begin CommandButton BtnAjuda 
      Caption         =   "Hilfe"
      Height          =   375
      Left            =   3000
      TabIndex        =   8
      Top             =   3180
      Width           =   1815
   End
   Begin CommandButton BtnCompact 
      Caption         =   "Komprimieren der Datenbank"
      Height          =   375
      Left            =   3000
      TabIndex        =   5
      Top             =   2580
      Width           =   3735
   End
   Begin CommandButton BtnInfPac 
      Caption         =   "Berichte über Sammlungen und Übungen"
      Height          =   375
      Left            =   3000
      TabIndex        =   4
      Top             =   1980
      Width           =   3735
   End
   Begin CommandButton BtnInfUsr 
      Caption         =   "Berichte über Benutzer"
      Height          =   375
      Left            =   3000
      TabIndex        =   3
      Top             =   1380
      Width           =   3735
   End
   Begin CommandButton BtnInfGrp 
      Caption         =   "Berichte über Benutzergruppen"
      Height          =   375
      Left            =   3000
      TabIndex        =   2
      Top             =   780
      Width           =   3735
   End
   Begin CommandButton BtnUsuaris 
      Caption         =   "Wartung der Benutzer und Gruppen"
      Height          =   375
      Left            =   3000
      TabIndex        =   1
      Top             =   180
      Width           =   3735
   End
   Begin CommandButton BtnAcabar 
      Caption         =   "Schließen"
      Height          =   375
      Left            =   4980
      TabIndex        =   0
      Top             =   3180
      Width           =   1755
   End
   Begin Label ver 
      BackStyle       =   0  'Transparent
      Caption         =   "Version 1.1"
      FontBold        =   0   'False
      FontItalic      =   0   'False
      FontName        =   "MS Sans Serif"
      FontSize        =   8.25
      FontStrikethru  =   0   'False
      FontUnderline   =   0   'False
      ForeColor       =   &H00000000&
      Height          =   255
      Left            =   240
      TabIndex        =   7
      Top             =   3360
      Width           =   2535
   End
   Begin Label titol 
      BackStyle       =   0  'Transparent
      Caption         =   "Führendes Modul der Datenbank der Berichte von Clic 3.0"
      FontBold        =   -1  'True
      FontItalic      =   0   'False
      FontName        =   "Arial"
      FontSize        =   9.75
      FontStrikethru  =   0   'False
      FontUnderline   =   0   'False
      ForeColor       =   &H00000000&
      Height          =   1035
      Left            =   240
      TabIndex        =   6
      Top             =   2220
      Width           =   2535
   End
   Begin Image logo 
      Height          =   1710
      Left            =   240
      Picture         =   CLICDB.FRX:0302
      Top             =   300
      Width           =   2505
   End
End
Option Explicit

Sub BtnAcabar_Click ()

Final

End Sub

Sub BtnAjuda_Click ()

SendKeys "{F1}"

End Sub

Sub BtnCompact_Click ()

Dim e As Integer

e = 0
db.Close
On Error GoTo compactErr
Set db = OpenDatabase(reportBase, True, False)
db.Close
On Error GoTo 0

If e Then
  MsgBox "Die Berichtsdatenbasis kann nicht komprimiert werden, da sie ein anderer Benutzer verwendet. Warten Sie bis alle ausgestiegen sind.", 48, "FEHLER"
Else
  ClicDB.Hide
  Compact.Show 1
  ClicDB.Show
End If

Set db = OpenDatabase(reportBase, False, True)

Exit Sub

compactErr:
e = 1
Resume Next

End Sub

Sub BtnInfGrp_Click ()

Load InfoGrp
ClicDB.Hide
InfoGrp.Show 1
ClicDB.Show

End Sub

Sub BtnInfPac_Click ()

Load InfoPac
ClicDB.Hide
InfoPac.Show 1
ClicDB.Show

End Sub

Sub BtnInfUsr_Click ()

Load InfoUsr
ClicDB.Hide
InfoUsr.Show 1
ClicDB.Show

End Sub

Sub BtnUsuaris_Click ()

Dim DesDe0 As Variant
Dim FinsA0 As Variant

DesDe0 = DesDe
FinsA0 = FinsA
db.Close
Set db = OpenDatabase(reportBase, False, False)
Load Usuaris
ClicDB.Hide
Usuaris.Show 1
ClicDB.Show
db.Close
Set db = OpenDatabase(reportBase, False, True)
DesDe = DesDe0
FinsA = FinsA0

End Sub

Sub Form_Load ()

CentrarImatge ClicDB
app.HelpFile = app.Path + "\clicdb.hlp"
If initGlobalVars() = False Then End
If directToUser <> 0 Then
  Visible = False
  BtnInfUsr_Click
  Final
End If
End Sub

Sub Form_Unload (Cancel As Integer)

Final

End Sub

