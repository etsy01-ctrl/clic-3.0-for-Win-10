VERSION 2.00
Begin Form Ident 
   BackColor       =   &H00C0C0C0&
   BorderStyle     =   1  'Fixed Single
   Caption         =   "CLIC: User ID"
   ClientHeight    =   6015
   ClientLeft      =   690
   ClientTop       =   1260
   ClientWidth     =   9375
   ControlBox      =   0   'False
   Height          =   6420
   HelpContextID   =   290
   Icon            =   IDENT.FRX:0000
   Left            =   630
   LinkTopic       =   "Form1"
   MaxButton       =   0   'False
   MinButton       =   0   'False
   ScaleHeight     =   6015
   ScaleWidth      =   9375
   Top             =   915
   Width           =   9495
   Begin CommandButton BtnNouUsuari 
      Caption         =   "New user"
      Enabled         =   0   'False
      Height          =   375
      Left            =   4800
      TabIndex        =   6
      Top             =   5580
      Width           =   2115
   End
   Begin CommandButton BtnAjuda 
      Caption         =   "Help"
      Height          =   375
      Left            =   7140
      TabIndex        =   7
      Top             =   5580
      Width           =   2175
   End
   Begin VScrollBar VScrollGrups 
      Enabled         =   0   'False
      Height          =   4639
      LargeChange     =   3
      Left            =   2235
      TabIndex        =   9
      Top             =   60
      Width           =   255
   End
   Begin VScrollBar VScrollUsr 
      Enabled         =   0   'False
      Height          =   5415
      LargeChange     =   3
      Left            =   9070
      TabIndex        =   8
      Top             =   60
      Width           =   255
   End
   Begin PictureBox FrameUsr 
      BackColor       =   &H00C0C0C0&
      FillColor       =   &H00C0C0C0&
      FillStyle       =   0  'Solid
      Height          =   5415
      Left            =   2580
      ScaleHeight     =   5385
      ScaleWidth      =   6465
      TabIndex        =   4
      Top             =   60
      Width           =   6495
      Begin SSCommand BtnUsr 
         Caption         =   "Any user"
         FontBold        =   0   'False
         FontItalic      =   0   'False
         FontName        =   "MS Sans Serif"
         FontSize        =   8.25
         FontStrikethru  =   0   'False
         FontUnderline   =   0   'False
         Height          =   375
         Index           =   0
         Left            =   0
         Outline         =   0   'False
         TabIndex        =   5
         Top             =   0
         Width           =   2535
      End
   End
   Begin PictureBox FrameGrup 
      BackColor       =   &H00C0C0C0&
      Height          =   4639
      Left            =   60
      ScaleHeight     =   4605
      ScaleWidth      =   2145
      TabIndex        =   2
      Top             =   60
      Width           =   2175
      Begin SSCommand BtnGrup 
         Caption         =   "Generic"
         FontBold        =   0   'False
         FontItalic      =   0   'False
         FontName        =   "MS Sans Serif"
         FontSize        =   8.25
         FontStrikethru  =   0   'False
         FontUnderline   =   0   'False
         Height          =   375
         Index           =   0
         Left            =   0
         Outline         =   0   'False
         TabIndex        =   3
         Top             =   0
         Width           =   2175
      End
   End
   Begin CommandButton BtnCancel 
      Cancel          =   -1  'True
      Caption         =   "Cancel"
      Height          =   375
      Left            =   2460
      TabIndex        =   1
      Top             =   5580
      Width           =   2115
   End
   Begin CommandButton BtnAcceptar 
      Caption         =   "OK"
      Default         =   -1  'True
      Enabled         =   0   'False
      Height          =   375
      Left            =   60
      TabIndex        =   0
      Top             =   5580
      Width           =   2175
   End
   Begin Label LabelUsr 
      BackColor       =   &H00FFFFFF&
      BackStyle       =   0  'Transparent
      Caption         =   "BBB"
      FontBold        =   0   'False
      FontItalic      =   0   'False
      FontName        =   "MS Sans Serif"
      FontSize        =   8.25
      FontStrikethru  =   0   'False
      FontUnderline   =   0   'False
      ForeColor       =   &H00FF0000&
      Height          =   375
      Left            =   600
      TabIndex        =   10
      Top             =   5160
      Width           =   1935
      WordWrap        =   -1  'True
   End
   Begin Label Label2 
      BackColor       =   &H00FFFFFF&
      BackStyle       =   0  'Transparent
      Caption         =   "User:"
      ForeColor       =   &H00000000&
      Height          =   255
      Left            =   60
      TabIndex        =   11
      Top             =   5160
      Width           =   495
   End
   Begin Label LabelGrup 
      BackColor       =   &H00FFFFFF&
      BackStyle       =   0  'Transparent
      Caption         =   "AAA"
      FontBold        =   0   'False
      FontItalic      =   0   'False
      FontName        =   "MS Sans Serif"
      FontSize        =   8.25
      FontStrikethru  =   0   'False
      FontUnderline   =   0   'False
      ForeColor       =   &H00FF0000&
      Height          =   195
      Left            =   720
      TabIndex        =   12
      Top             =   4920
      Width           =   1815
   End
   Begin Label Label1 
      BackColor       =   &H00FFFFFF&
      BackStyle       =   0  'Transparent
      Caption         =   "Group:"
      ForeColor       =   &H00000000&
      Height          =   255
      Left            =   60
      TabIndex        =   13
      Top             =   4920
      Width           =   615
   End
End
Option Explicit

Dim numGroups As Integer
Dim numUsers As Integer

Dim numUsrMax As Integer
Dim numGrpMax As Integer

Dim cGroup As Long
Dim cUser As Long

Dim grpHeight As Integer
Dim grpWidth As Integer
Dim usrHeight As Integer
Dim usrWidth As Integer
Dim usrListPos As Integer
Dim grpListPos As Integer

Dim dblClic As Integer

Const usrCols = 6
Const grpCols = 2
Const usrRows = 15
Const grpRows = 12

Sub Acaba ()

Unload Ident

End Sub

Sub BtnAcceptar_Click ()

If cGroup = 0 Or cUser = 0 Then
  MsgBox "Must select a user name!"
  Exit Sub
End If

currentGroup = cGroup
currentUser = cUser

Acaba

End Sub

Sub BtnAjuda_Click ()

SendKeys "{F1}"

End Sub

Sub BtnCancel_Click ()

currentGroup = 0
currentUser = 0
Acaba

End Sub

Sub BtnGrup_click (index As Integer)

Dim i As Integer
Dim t As String

For i = 0 To numGroups - 1
  BtnGrup(i).Outline = False
  BtnGrup(i).ForeColor = FORE1
Next i
BtnGrup(index).Outline = True
BtnGrup(index).ForeColor = FORE2
t = BtnGrup(index).Tag
cGroup = Val(t)
LabelUsr = ""
currentGroupName = Right(t, Len(t) - InStr(t, "-"))
LabelGrup = currentGroupName
BtnAcceptar.Enabled = False

While BtnGrup(index).Top < 0 And VScrollGrups.Value > VScrollGrups.Min
  VScrollGrups.Value = VScrollGrups.Value - 1
Wend
While (BtnGrup(index).Top + BtnGrup(index).Height) > frameGrup.Height And VScrollGrups.Value < VScrollGrups.Max
  VScrollGrups.Value = VScrollGrups.Value + 1
Wend

FillUsrList

End Sub

Sub BtnNouUsuari_Click ()

Dim i As Integer
Dim t As String

currentUserName = ""
NewUsr.Show 1

If currentUserName <> "" Then
  FillGrpList
  For i = 0 To numGroups - 1
    t = BtnGrup(i).Tag
    If currentGroupName = Right(t, Len(t) - InStr(t, "-")) Then Exit For
  Next i
  If i < numGroups Then
    BtnGrup_click (i)
    If numUsers > 0 Then
      For i = 0 To numUsers - 1
	t = BtnUsr(i).Tag
	If currentUserName = Right(t, Len(t) - InStr(t, "-")) Then
	  BtnUsr_Click (i)
	  Exit For
	End If
      Next i
    End If
  End If
End If
  
End Sub

Sub BtnUsr_Click (index As Integer)

Dim i As Integer
Dim s As String

If dblClic = True And BtnUsr(index).Outline = True Then
  BtnAcceptar_Click
End If

For i = 0 To numUsrMax
  BtnUsr(i).Outline = False
  BtnUsr(i).ForeColor = FORE1
Next i

s = BtnUsr(index).Tag
cUser = Val(s)
LabelUsr = Right(s, Len(s) - InStr(s, "-"))
BtnUsr(index).Outline = True
BtnUsr(index).ForeColor = FORE2
BtnAcceptar.Enabled = True

While BtnUsr(index).Top < 0 And VScrollUsr.Value > VScrollUsr.Min
  VScrollUsr.Value = VScrollUsr.Value - 1
Wend
While (BtnUsr(index).Top + BtnUsr(index).Height) > frameUsr.Height And VScrollUsr.Value < VScrollUsr.Max
  VScrollUsr.Value = VScrollUsr.Value + 1
Wend

End Sub

Sub FillGrpList ()

Dim d As dynaset
Dim s As Variant
Dim i As Integer, j As Integer
Dim y As Integer
Static t(grpCols, 3) As Integer
Dim xp As Integer, yp As Integer
Dim w As Integer, h As Integer
Dim found As Integer

numGroups = 0

For xp = 0 To grpCols - 1
  For yp = 0 To 2
    t(xp, yp) = 0
  Next yp
Next xp
y = 0

For i = 0 To numGrpMax
  BtnGrup(i).Visible = False
Next i

Screen.MousePointer = 11
Set d = db.CreateDynaset("SELECT * FROM groups ORDER BY groupName", DB_READONLY)
d.MoveFirst
Do Until d.EOF
  If numGroups > numGrpMax Then
    Load BtnGrup(numGroups)
    numGrpMax = numGroups
  End If
  BtnGrup(numGroups).Outline = False
  BtnGrup(numGroups).ForeColor = FORE1
  BtnGrup(numGroups).Tag = d("groupId") & "-" & d("groupName")
  s = d("groupImg")
  If s <> "" Then
    On Error GoTo fillGrpErr
    BtnGrup(numGroups).Picture = LoadPicture(s)
    On Error GoTo 0
  End If
  found = False
  If s <> "" Then
    BtnGrup(numGroups).Caption = ""
    w = 1: h = 3: yp = 0
    For xp = 0 To grpCols - 1
      If t(xp, 0) + t(xp, 1) + t(xp, 2) = 0 Then found = True
      If found Then Exit For
    Next xp
  Else
    BtnGrup(numGroups).Caption = d("groupName")
    BtnGrup(numGroups).Picture = LoadPicture()
    w = 2: h = 1
    found = False
    For yp = 0 To 2
      For xp = 0 To grpCols - 2
	If t(xp, yp) + t(xp + 1, yp) = 0 Then found = True
	If found Then Exit For
      Next xp
      If found Then Exit For
    Next yp
  End If
  If found = False Then
    j = 0
    For yp = 2 To 0 Step -1
      For xp = 0 To grpCols - 1
	j = j + t(xp, yp)
	t(xp, yp) = 0
      Next xp
      If j <> 0 Then y = y + 1
    Next yp
    xp = 0: yp = 0
  End If
  BtnGrup(numGroups).Width = w * grpWidth
  BtnGrup(numGroups).Height = h * grpHeight
  BtnGrup(numGroups).Top = (y + yp) * grpHeight
  BtnGrup(numGroups).Left = xp * grpWidth
  For i = xp To xp + w - 1
    For j = yp To yp + h - 1
      t(i, j) = 1
    Next j
  Next i
  BtnGrup(numGroups).Visible = True
  numGroups = numGroups + 1
  d.MoveNext
Loop

d.Close

grpListPos = 0
If y < grpRows - 3 Then
  VScrollGrups.Enabled = False
Else
  VScrollGrups.Min = 0
  VScrollGrups.Max = y - grpRows + 3
  VScrollGrups.Value = 0
  VScrollGrups.Enabled = True
End If

Screen.MousePointer = 0

Exit Sub

fillGrpErr:
s = ""
Resume Next

End Sub

Sub FillUsrList ()

Dim d As dynaset
Dim c As String
Dim s As Variant
Dim i As Integer, j As Integer
Dim y As Integer
Static t(usrCols, 3) As Integer
Dim xp As Integer, yp As Integer
Dim w As Integer, h As Integer
Dim found As Integer

cUser = 0
numUsers = 0

For xp = 0 To usrCols - 1
  For yp = 0 To 2
    t(xp, yp) = 0
  Next yp
Next xp
y = 0

For i = 0 To numUsrMax
  BtnUsr(i).Visible = False
Next i

Screen.MousePointer = 11
Set d = db.CreateDynaset("SELECT * FROM users ORDER BY userName", DB_READONLY)
c = "groupId = " & cGroup
d.MoveFirst
d.FindFirst c
Do Until d.NoMatch
  If numUsers > numUsrMax Then
    Load BtnUsr(numUsers)
    numUsrMax = numUsers
  End If
  BtnUsr(numUsers).Outline = False
  BtnUsr(numUsers).ForeColor = FORE1
  BtnUsr(numUsers).Tag = d("userId") & "-" & d("userName")
  s = d("userImg")
  If s <> "" Then
    On Error GoTo fillUsrErr
    BtnUsr(numUsers).Picture = LoadPicture(s)
    On Error GoTo 0
  End If
  found = False
  If s <> "" Then
    BtnUsr(numUsers).Caption = ""
    w = 1: h = 3: yp = 0
    For xp = 0 To usrCols - 1
      If t(xp, 0) + t(xp, 1) + t(xp, 2) = 0 Then found = True
      If found Then Exit For
    Next xp
  Else
    BtnUsr(numUsers).Caption = d("userName")
    BtnUsr(numUsers).Picture = LoadPicture()
    w = 2: h = 1
    found = False
    For yp = 0 To 2
      For xp = 0 To usrCols - 2
	If t(xp, yp) + t(xp + 1, yp) = 0 Then found = True
	If found Then Exit For
      Next xp
      If found Then Exit For
    Next yp
  End If
  If found = False Then
    j = 0
    For yp = 2 To 0 Step -1
      For xp = 0 To usrCols - 1
	j = j + t(xp, yp)
	t(xp, yp) = 0
      Next xp
      If j <> 0 Then y = y + 1
    Next yp
    xp = 0: yp = 0
  End If
  BtnUsr(numUsers).Width = w * usrWidth
  BtnUsr(numUsers).Height = h * usrHeight
  BtnUsr(numUsers).Top = (y + yp) * usrHeight
  BtnUsr(numUsers).Left = xp * usrWidth
  For i = xp To xp + w - 1
    For j = yp To yp + h - 1
      t(i, j) = 1
    Next j
  Next i
  BtnUsr(numUsers).Visible = True
  numUsers = numUsers + 1
  d.FindNext c
Loop

d.Close

usrListPos = 0
If y < usrRows - 3 Then
  VScrollUsr.Enabled = False
Else
  VScrollUsr.Min = 0
  VScrollUsr.Max = y - usrRows + 3
  VScrollUsr.Value = 0
  VScrollUsr.Enabled = True
End If

Screen.MousePointer = 0

Exit Sub

fillUsrErr:
s = ""
Resume Next

End Sub

Sub Form_Load ()

Dim i As Integer
Dim cu0 As Long

dblClic = False

Left = (Screen.Width - Width) / 2
Top = (Screen.Height - Height) / 2

If newUsersAllowed Then BtnNouUsuari.Enabled = True

numGroups = 0
numUsers = 0
numUsrMax = 0
numGrpMax = 0
grpHeight = frameGrup.Height / grpRows
grpWidth = frameGrup.Width / grpCols
usrHeight = frameUsr.Height / usrRows
usrWidth = frameUsr.Width / usrCols

FillGrpList

cGroup = currentGroup
If cGroup = 0 Then cGroup = 1
cUser = currentUser
If cUser = 0 Then cUser = 1

If numGroups > 0 Then
  For i = 0 To numGroups - 1
    If Val(BtnGrup(i).Tag) = cGroup Then Exit For
  Next i
  If i < numGroups Then
    cu0 = cUser
    BtnGrup_click (i)
    If numUsers > 0 Then
      For i = 0 To numUsers - 1
	If Val(BtnUsr(i).Tag) = cu0 Then
	  BtnUsr_Click (i)
	  Exit For
	End If
      Next i
    End If
  End If
End If

dblClic = True

End Sub

Sub VScrollGrups_Change ()

  Dim delta As Integer, i As Integer

  delta = grpListPos - VScrollGrups.Value
  If delta <> 0 Then
    For i = 0 To numGroups - 1
      BtnGrup(i).Top = BtnGrup(i).Top + delta * grpHeight
    Next i
  End If

  grpListPos = VScrollGrups.Value
  
End Sub

Sub VScrollUsr_Change ()
  
  Dim delta As Integer, i As Integer

  delta = usrListPos - VScrollUsr.Value
  If delta <> 0 Then
    For i = 0 To numUsers - 1
      BtnUsr(i).Top = BtnUsr(i).Top + delta * usrHeight
    Next i
  End If

  usrListPos = VScrollUsr.Value
  
End Sub

