VERSION 2.00
Begin Form NewUsr 
   BackColor       =   &H00C0C0C0&
   BorderStyle     =   3  'Fixed Double
   Caption         =   "Nou usuari"
   ClientHeight    =   1695
   ClientLeft      =   3075
   ClientTop       =   3000
   ClientWidth     =   5640
   ControlBox      =   0   'False
   Height          =   2100
   HelpContextID   =   290
   Icon            =   0
   Left            =   3015
   LinkTopic       =   "Form1"
   MaxButton       =   0   'False
   MinButton       =   0   'False
   ScaleHeight     =   1695
   ScaleWidth      =   5640
   Top             =   2655
   Width           =   5760
   Begin CommandButton BtnNouGrup 
      Caption         =   "Nou grup..."
      Enabled         =   0   'False
      Height          =   315
      Left            =   4020
      TabIndex        =   7
      Top             =   180
      Width           =   1515
   End
   Begin ListBox listIdGroups 
      Height          =   420
      Left            =   120
      TabIndex        =   6
      Top             =   1200
      Visible         =   0   'False
      Width           =   435
   End
   Begin ComboBox listGroups 
      ForeColor       =   &H000000FF&
      Height          =   315
      Left            =   780
      Style           =   2  'Dropdown List
      TabIndex        =   4
      Top             =   180
      Width           =   3135
   End
   Begin TextBox itemName 
      Height          =   315
      Left            =   780
      MaxLength       =   80
      TabIndex        =   2
      Top             =   660
      Width           =   4755
   End
   Begin CommandButton BtnCancel 
      Cancel          =   -1  'True
      Caption         =   "Cancel·lar"
      Height          =   375
      Left            =   3240
      TabIndex        =   1
      Top             =   1140
      Width           =   1455
   End
   Begin CommandButton BtnAcceptar 
      Caption         =   "D'Acord"
      Default         =   -1  'True
      Height          =   375
      Left            =   1320
      TabIndex        =   0
      Top             =   1140
      Width           =   1455
   End
   Begin Label Label2 
      BackColor       =   &H00C0C0C0&
      Caption         =   "Grup:"
      Height          =   255
      Left            =   60
      TabIndex        =   5
      Top             =   240
      Width           =   675
   End
   Begin Label Label1 
      BackStyle       =   0  'Transparent
      Caption         =   "Nom:"
      Height          =   255
      Left            =   60
      TabIndex        =   3
      Top             =   720
      Width           =   735
   End
End
Option Explicit

Sub BtnAcceptar_Click ()

Dim msg As String
Dim u As Dynaset
Dim s As String

msg = ""
If itemName = "" Then
  msg = "No es pot deixar el nom en blanc!"
Else
  If InStr(itemName, Chr(34)) Then
    msg = "No es poden utilitzar les cometes en els noms!"
  End If
End If

If msg <> "" Then
  MsgBox msg, 48
  Exit Sub
End If

Screen.MousePointer = 11
Set u = db.CreateDynaset("SELECT * FROM users WHERE groupId=" & listIdGroups.List(listGroups.ListIndex))
s = "userName=" & Chr(34) & itemName & Chr(34)
u.FindFirst s
If u.NoMatch = False Then
  msg = "Ja hi ha un altre usuari amb aquest nom en el grup!"
  u.Close
  Screen.MousePointer = 0
  MsgBox msg, 48
  Exit Sub
End If

u.AddNew
u("userName") = itemName
u("userImg") = ""
u("groupId") = listIdGroups.List(listGroups.ListIndex)
u.Update
u.Close
Screen.MousePointer = 0

currentUserName = itemName
currentGroupName = listGroups.List(listGroups.ListIndex)
Unload NewUsr

End Sub

Sub BtnCancel_Click ()

Unload NewUsr

End Sub

Sub BtnNouGrup_Click ()

Dim cgn0 As String
cgn0 = currentGroupName

NewGrp.Show 1

If cgn0 <> currentGroupName Then
  FillListGroups
End If

End Sub

Sub FillListGroups ()

Dim u As Dynaset
Dim i As Integer

listGroups.Clear
listIdGroups.Clear

Screen.MousePointer = 11
Set u = db.CreateDynaset("SELECT * FROM groups ORDER BY groupName")
Do Until u.EOF
  listGroups.AddItem u("groupName")
  listIdGroups.AddItem u("groupId")
  u.MoveNext
Loop

i = 0
If currentGroupName <> "" Then
  For i = 0 To listGroups.ListCount - 1
    If listGroups.List(i) = currentGroupName Then Exit For
  Next i
  If i = listGroups.ListCount Then i = 0
End If

listGroups.ListIndex = i

u.Close
Screen.MousePointer = 0

End Sub

Sub Form_Load ()

NewUsr.Icon = Ident.Icon
Left = (Screen.Width - Width) / 2
Top = (Screen.Height - Height) / 2
currentUserName = ""
If newGroupsAllowed Then BtnNouGrup.enabled = True

FillListGroups

End Sub

