VERSION 2.00
Begin Form Usuaris 
   BackColor       =   &H00C0C0C0&
   BorderStyle     =   3  'Fixed Double
   Caption         =   "Talde eta erabiltzaileen mantenimendua"
   ClientHeight    =   5835
   ClientLeft      =   1095
   ClientTop       =   1665
   ClientWidth     =   7995
   ControlBox      =   0   'False
   Height          =   6240
   HelpContextID   =   400
   Icon            =   0
   Left            =   1035
   LinkTopic       =   "Form2"
   MaxButton       =   0   'False
   ScaleHeight     =   5835
   ScaleWidth      =   7995
   Top             =   1320
   Width           =   8115
   Begin CommandButton BtnAjuda 
      Caption         =   "Laguntza"
      Height          =   375
      Left            =   4080
      TabIndex        =   15
      Top             =   5340
      Width           =   1755
   End
   Begin CommandButton BtnEditUser 
      Caption         =   "Ezaugarriak..."
      Enabled         =   0   'False
      FontBold        =   0   'False
      FontItalic      =   0   'False
      FontName        =   "MS Sans Serif"
      FontSize        =   8.25
      FontStrikethru  =   0   'False
      FontUnderline   =   0   'False
      Height          =   315
      Left            =   4260
      TabIndex        =   14
      Top             =   3360
      Width           =   3495
   End
   Begin CommandButton BtnEditGroup 
      Caption         =   "Ezaugarriak..."
      Enabled         =   0   'False
      FontBold        =   0   'False
      FontItalic      =   0   'False
      FontName        =   "MS Sans Serif"
      FontSize        =   8.25
      FontStrikethru  =   0   'False
      FontUnderline   =   0   'False
      Height          =   315
      Left            =   180
      TabIndex        =   13
      Top             =   3360
      Width           =   3495
   End
   Begin CommandButton BtnNetejaDadesUsr 
      Caption         =   "Erabiltzaile honen datuak garbitu"
      Enabled         =   0   'False
      FontBold        =   0   'False
      FontItalic      =   0   'False
      FontName        =   "MS Sans Serif"
      FontSize        =   8.25
      FontStrikethru  =   0   'False
      FontUnderline   =   0   'False
      Height          =   315
      Left            =   4260
      TabIndex        =   7
      Top             =   4200
      Width           =   3495
   End
   Begin CommandButton BtnNouUsr 
      Caption         =   "Talde honetan erabiltzaile berri bat sortu"
      Enabled         =   0   'False
      FontBold        =   0   'False
      FontItalic      =   0   'False
      FontName        =   "MS Sans Serif"
      FontSize        =   8.25
      FontStrikethru  =   0   'False
      FontUnderline   =   0   'False
      Height          =   315
      Left            =   4260
      TabIndex        =   8
      Top             =   3780
      Width           =   3495
   End
   Begin CommandButton BtnElimUsr 
      Caption         =   "Erabiltzaile hau ezabatu"
      Enabled         =   0   'False
      FontBold        =   0   'False
      FontItalic      =   0   'False
      FontName        =   "MS Sans Serif"
      FontSize        =   8.25
      FontStrikethru  =   0   'False
      FontUnderline   =   0   'False
      Height          =   315
      Left            =   4260
      TabIndex        =   11
      Top             =   4620
      Width           =   3495
   End
   Begin ListBox ListIdUsers 
      Height          =   420
      Left            =   6060
      TabIndex        =   10
      Top             =   5100
      Visible         =   0   'False
      Width           =   1155
   End
   Begin ListBox ListUsers 
      ForeColor       =   &H00FF0000&
      Height          =   2760
      Left            =   4260
      TabIndex        =   9
      Top             =   480
      Width           =   3495
   End
   Begin CommandButton BtnNouGrup 
      Caption         =   "Talde berria sortu"
      FontBold        =   0   'False
      FontItalic      =   0   'False
      FontName        =   "MS Sans Serif"
      FontSize        =   8.25
      FontStrikethru  =   0   'False
      FontUnderline   =   0   'False
      Height          =   315
      Left            =   180
      TabIndex        =   6
      Top             =   3780
      Width           =   3495
   End
   Begin CommandButton BtnEliminaGrup 
      Caption         =   "Talde hau ezabatu"
      Enabled         =   0   'False
      FontBold        =   0   'False
      FontItalic      =   0   'False
      FontName        =   "MS Sans Serif"
      FontSize        =   8.25
      FontStrikethru  =   0   'False
      FontUnderline   =   0   'False
      Height          =   315
      Left            =   180
      TabIndex        =   5
      Top             =   4620
      Width           =   3495
   End
   Begin CommandButton BtnNetejaDadesGrup 
      Caption         =   "Talde honen datuak garbitu"
      Enabled         =   0   'False
      FontBold        =   0   'False
      FontItalic      =   0   'False
      FontName        =   "MS Sans Serif"
      FontSize        =   8.25
      FontStrikethru  =   0   'False
      FontUnderline   =   0   'False
      Height          =   315
      Left            =   180
      TabIndex        =   4
      Top             =   4200
      Width           =   3495
   End
   Begin CommandButton BtnAcaba 
      Caption         =   "Ados"
      Height          =   375
      Left            =   2100
      TabIndex        =   3
      Top             =   5340
      Width           =   1755
   End
   Begin ListBox ListIdGroups 
      Height          =   420
      Left            =   720
      TabIndex        =   2
      Top             =   5040
      Visible         =   0   'False
      Width           =   1215
   End
   Begin ListBox ListGroups 
      ForeColor       =   &H000000FF&
      Height          =   2760
      Left            =   180
      TabIndex        =   1
      Top             =   480
      Width           =   3495
   End
   Begin Image Image1 
      Height          =   135
      Left            =   3780
      Picture         =   USUARIS.FRX:0000
      Top             =   540
      Width           =   390
   End
   Begin Label LbUsr 
      BackStyle       =   0  'Transparent
      Caption         =   "Erabiltzaileak:"
      ForeColor       =   &H00000000&
      Height          =   255
      Left            =   4260
      TabIndex        =   12
      Top             =   240
      Width           =   3495
   End
   Begin Label Label1 
      BackStyle       =   0  'Transparent
      Caption         =   "Taldeak:"
      ForeColor       =   &H00000000&
      Height          =   255
      Left            =   180
      TabIndex        =   0
      Top             =   240
      Width           =   3495
   End
End
Option Explicit

Dim currentGroup As Integer
Dim currentUser As Integer

Sub BtnAcaba_Click ()

Unload Usuaris

End Sub

Sub BtnAjuda_Click ()

SendKeys "{F1}"

End Sub

Sub BtnEditGroup_Click ()

editGroup = True
editItem = Val(ListIdGroups.List(currentGroup))
EditUsr.Show 1
If Not cancelEdit Then FillListGroups

End Sub

Sub BtnEditUser_Click ()

editGroup = False
editItem = Val(ListIdUsers.List(currentUser))
editGroupId = Val(ListIdGroups.List(currentGroup))
EditUsr.Show 1
If Not cancelEdit Then FillListUsers

End Sub

Sub BtnEliminaGrup_Click ()

Dim desDe00 As Variant
Dim finsA00 As Variant
Dim cg As String

cg = ListIdGroups.List(currentGroup)
If Val(cg) < 2 Then
  MsgBox "Ezin da talde hau ezabatu!", 48
  Exit Sub
End If

If MsgBox("Operazio honek talde honen erabiltzaile guztien datuak ezabatuko ditu. Ziur zaude jarraitu nahi duzula?", 4 + 48, "ADI !!") <> 6 Then Exit Sub

Usuaris.Refresh

screen.MousePointer = 11

desDe00 = desDe
finsA00 = finsA
desDe = CVDate(0)
finsA = CVDate(2000000)
NetejaSessions cg, "*"
db.Execute "DELETE FROM users WHERE groupId=" & cg
db.Execute "DELETE FROM groups WHERE groupId=" & cg
desDe = desDe00
finsA = finsA00
screen.MousePointer = 0

currentGroup = currentGroup - 1
FillListGroups

End Sub

Sub BtnElimUsr_Click ()

Dim desDe00 As Variant
Dim finsA00 As Variant
Dim usr As Long

usr = Val(ListIdUsers.List(currentUser))

If currentGroup < 1 And usr < 2 Then
  MsgBox "Ezin da erabiltzaile hau ezabatu!", 48
  Exit Sub
End If

If MsgBox("Ziur zaude erabiltzaile hau betirako ezabatu nahi duzula?", 4 + 48, "ADI !!") <> 6 Then Exit Sub

Usuaris.Refresh

desDe00 = desDe
finsA00 = finsA
desDe = CVDate(0)
finsA = CVDate(2000000)
NetejaSessions ListIdGroups.List(currentGroup), Str(usr)
desDe = desDe00
finsA = finsA00
db.Execute "DELETE FROM users WHERE userId=" & usr
screen.MousePointer = 0

currentUser = currentUser - 1
If currentUser < 0 Then currentUser = 0
FillListUsers

End Sub

Sub BtnNetejaDadesGrup_Click ()

If MsgBox("Operazio honek talde honen erabiltzaile guztien datuak garbituko ditu. Ziur zaude jarraitu nahi duzula?", 4 + 48, "ADI !!") <> 6 Then Exit Sub

Dates.Show 1

If cancelData = False Then
  screen.MousePointer = 11
  Usuaris.Refresh
  NetejaSessions ListIdGroups.List(currentGroup), "*"
  screen.MousePointer = 0
End If


End Sub

Sub BtnNetejaDadesUsr_Click ()

If MsgBox("Operazio honek erabiltzaile honen datu guztiak garbituko ditu. Ziur zaude?", 4 + 48, "ADI !!") <> 6 Then Exit Sub

Dates.Show 1

If cancelData = False Then
  screen.MousePointer = 11
  Usuaris.Refresh
  NetejaSessions ListIdGroups.List(currentGroup), ListIdUsers.List(ListUsers.ListIndex)
  screen.MousePointer = 0
End If

End Sub

Sub BtnNouGrup_Click ()

Dim i As Integer

editGroup = True
editItem = 0
EditUsr.Show 1
If Not cancelEdit Then
  'currentGroup = ListGroups.ListCount
  currentGroup = -1
  FillListGroups
  For i = 0 To ListGroups.ListCount - 1
    If ListGroups.List(i) = lastName Then Exit For
  Next i
  If i < ListGroups.ListCount Then
    ListGroups.ListIndex = i
  End If
End If

End Sub

Sub BtnNouUsr_Click ()

Dim i As Integer

editGroup = False
editItem = 0
editGroupId = Val(ListIdGroups.List(currentGroup))
EditUsr.Show 1
If Not cancelEdit Then
  'currentUser = ListUsers.ListCount
  currentUser = -1
  FillListUsers
  For i = 0 To ListUsers.ListCount - 1
    If ListUsers.List(i) = lastName Then Exit For
  Next i
  If i < ListUsers.ListCount Then
    ListUsers.ListIndex = i
  End If
End If

End Sub

Sub FillListGroups ()

Dim u As dynaset

ListGroups.Clear
ListIdGroups.Clear

Set u = db.CreateDynaset("SELECT * FROM groups ORDER BY groupName")
Do Until u.EOF
  ListGroups.AddItem u("groupName")
  ListIdGroups.AddItem u("groupId")
  u.MoveNext
Loop

If ListGroups.ListCount > currentGroup Then
  ListGroups.ListIndex = currentGroup
End If

u.Close

End Sub

Sub FillListUsers ()

Dim u As dynaset
Dim i As Integer

ListUsers.Clear
ListIdUsers.Clear
BtnNetejaDadesUsr.Enabled = False
BtnElimUsr.Enabled = False
BtnEditUser.Enabled = False

Set u = db.CreateDynaset("SELECT * FROM users WHERE (groupId=" & ListIdGroups.List(currentGroup) & ") ORDER BY userName")
If u.EOF Then

Else
  Do Until u.EOF
    ListUsers.AddItem u("userName")
    ListIdUsers.AddItem u("userId")
    u.MoveNext
  Loop
  ListUsers.ListIndex = currentUser
End If

u.Close

End Sub

Sub Form_Load ()

Usuaris.Icon = ClicDB.Icon
CentrarImatge Usuaris

currentGroup = 0
currentUser = 0
FillListGroups

End Sub

Sub ListGroups_Click ()

Dim u As dynaset

currentGroup = ListGroups.ListIndex
currentUser = 0
FillListUsers
BtnNouUsr.Enabled = True
BtnNetejaDadesGrup.Enabled = True
BtnEliminaGrup.Enabled = True
BtnEditGroup.Enabled = True
LbUsr = "Honen erabiltzaileak: " & Chr(34) & ListGroups.List(currentGroup) & Chr(34) & ":"

End Sub

Sub ListUsers_Click ()

BtnNetejaDadesUsr.Enabled = True
BtnElimUsr.Enabled = True
BtnEditUser.Enabled = True
currentUser = ListUsers.ListIndex

End Sub

