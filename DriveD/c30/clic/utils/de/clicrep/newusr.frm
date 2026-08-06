VERSION 2.00
Begin Form NewUsr 
   BackColor       =   &H00C0C0C0&
   BorderStyle     =   3  'Fixed Double
   Caption         =   "neuer Benutzer"
   ClientHeight    =   2160
   ClientLeft      =   1815
   ClientTop       =   1950
   ClientWidth     =   5640
   ControlBox      =   0   'False
   Height          =   2565
   HelpContextID   =   290
   Icon            =   0
   Left            =   1755
   LinkTopic       =   "Form1"
   MaxButton       =   0   'False
   MinButton       =   0   'False
   ScaleHeight     =   2160
   ScaleWidth      =   5640
   Top             =   1605
   Width           =   5760
   Begin CommandButton BtnNouGrup 
      Caption         =   "Neue Benutzergruppe..."
      Enabled         =   0   'False
      Height          =   315
      Left            =   3120
      TabIndex        =   7
      Top             =   600
      Width           =   2415
   End
   Begin ListBox listIdGroups 
      Height          =   420
      Left            =   180
      TabIndex        =   6
      Top             =   1620
      Visible         =   0   'False
      Width           =   435
   End
   Begin ComboBox listGroups 
      ForeColor       =   &H000000FF&
      Height          =   315
      Left            =   1560
      Style           =   2  'Dropdown List
      TabIndex        =   4
      Top             =   180
      Width           =   3975
   End
   Begin TextBox itemName 
      Height          =   315
      Left            =   960
      MaxLength       =   80
      TabIndex        =   2
      Top             =   1020
      Width           =   4575
   End
   Begin CommandButton BtnCancel 
      Cancel          =   -1  'True
      Caption         =   "Abbrechen"
      Height          =   375
      Left            =   3300
      TabIndex        =   1
      Top             =   1560
      Width           =   1455
   End
   Begin CommandButton BtnAcceptar 
      Caption         =   "OK"
      Default         =   -1  'True
      Height          =   375
      Left            =   1380
      TabIndex        =   0
      Top             =   1560
      Width           =   1455
   End
   Begin Label Label2 
      BackColor       =   &H00C0C0C0&
      Caption         =   "Benutzergruppe:"
      Height          =   255
      Left            =   60
      TabIndex        =   5
      Top             =   240
      Width           =   1455
   End
   Begin Label Label1 
      BackStyle       =   0  'Transparent
      Caption         =   "Benutzer:"
      Height          =   255
      Left            =   60
      TabIndex        =   3
      Top             =   1080
      Width           =   855
   End
End
Option Explicit

Sub BtnAcceptar_Click ()

Dim msg As String
Dim u As Dynaset
Dim s As String

msg = ""
If itemName = "" Then
  msg = "Sie können das Namensfeld nicht leer lassen!"
Else
  If InStr(itemName, Chr(34)) Then
    msg = "Sei können keine Anführungszeichen im Namen verwenden!"
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
  msg = "Es gibt einen anderen Benutzer mit dem selben Namen in der Gruppe!"
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

