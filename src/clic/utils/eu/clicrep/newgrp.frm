VERSION 2.00
Begin Form NewGrp 
   BackColor       =   &H00C0C0C0&
   BorderStyle     =   3  'Fixed Double
   Caption         =   "Talde berria"
   ClientHeight    =   1230
   ClientLeft      =   2640
   ClientTop       =   3705
   ClientWidth     =   4125
   ControlBox      =   0   'False
   Height          =   1635
   HelpContextID   =   290
   Left            =   2580
   LinkTopic       =   "Form1"
   MaxButton       =   0   'False
   MinButton       =   0   'False
   ScaleHeight     =   1230
   ScaleWidth      =   4125
   Top             =   3360
   Width           =   4245
   Begin CommandButton BtnCancel 
      Cancel          =   -1  'True
      Caption         =   "Utzi"
      Height          =   375
      Left            =   2160
      TabIndex        =   3
      Top             =   780
      Width           =   1395
   End
   Begin CommandButton BtnAcceptar 
      Caption         =   "Ados"
      Default         =   -1  'True
      Height          =   375
      Left            =   420
      TabIndex        =   2
      Top             =   780
      Width           =   1395
   End
   Begin TextBox itemName 
      Height          =   285
      Left            =   120
      MaxLength       =   80
      TabIndex        =   0
      Top             =   360
      Width           =   3855
   End
   Begin Label Label1 
      BackStyle       =   0  'Transparent
      Caption         =   "Taldearen izena:"
      Height          =   255
      Left            =   120
      TabIndex        =   1
      Top             =   120
      Width           =   3795
   End
End
Option Explicit

Sub BtnAcceptar_Click ()

Dim msg As String
Dim u As Dynaset
Dim s As String

msg = ""
If itemName = "" Then
  msg = "Ezin da izena hutsik utzi!"
Else
  If InStr(itemName, Chr(34)) Then
    msg = "Ezin da kakotxarik erabili izenetan ¡"
  End If
End If

If msg <> "" Then
  MsgBox msg, 48
  Exit Sub
End If

Screen.MousePointer = 11
Set u = db.CreateDynaset("groups")
s = "groupName=" & Chr(34) & itemName & Chr(34)
u.FindFirst s
If u.NoMatch = False Then
  msg = "Badago beste izen bat izen berberarekin"
  u.Close
  Screen.MousePointer = 0
  MsgBox msg, 48
  Exit Sub
End If

u.AddNew
u("groupName") = itemName
u("groupImg") = ""
u.Update
u.Close

Screen.MousePointer = 0

currentGroupName = itemName

Unload NewGrp

End Sub

Sub BtnCancel_Click ()

Unload NewGrp

End Sub

Sub Form_Load ()

NewGrp.icon = Ident.icon
Left = (Screen.Width - Width) / 2
Top = (Screen.Height - Height) / 2

End Sub

