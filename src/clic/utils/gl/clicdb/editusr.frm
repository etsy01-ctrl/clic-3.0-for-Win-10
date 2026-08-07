VERSION 2.00
Begin Form EditUsr 
   BackColor       =   &H00C0C0C0&
   BorderStyle     =   3  'Fixed Double
   Caption         =   "Propiedades"
   ClientHeight    =   3825
   ClientLeft      =   2310
   ClientTop       =   2100
   ClientWidth     =   4815
   ControlBox      =   0   'False
   Height          =   4230
   HelpContextID   =   400
   Left            =   2250
   LinkTopic       =   "Form1"
   MaxButton       =   0   'False
   MinButton       =   0   'False
   ScaleHeight     =   3825
   ScaleWidth      =   4815
   Top             =   1755
   Width           =   4935
   Begin CommandButton BtnAjuda 
      Cancel          =   -1  'True
      Caption         =   "Axuda"
      Height          =   375
      Left            =   3300
      TabIndex        =   9
      Top             =   3360
      Width           =   1455
   End
   Begin TextBox itemName 
      DataField       =   "groupName"
      DataSource      =   "DtGrups"
      Height          =   285
      Left            =   180
      MaxLength       =   80
      TabIndex        =   1
      Top             =   360
      Width           =   4395
   End
   Begin TextBox itemImg 
      DataField       =   "groupImg"
      DataSource      =   "DtGrups"
      FontBold        =   0   'False
      FontItalic      =   0   'False
      FontName        =   "MS Sans Serif"
      FontSize        =   8.25
      FontStrikethru  =   0   'False
      FontUnderline   =   0   'False
      Height          =   285
      Left            =   180
      MaxLength       =   200
      TabIndex        =   2
      Top             =   1260
      Width           =   4395
   End
   Begin CommandButton BtnCercaImg 
      Caption         =   "Buscar imaxe..."
      FontBold        =   0   'False
      FontItalic      =   0   'False
      FontName        =   "MS Sans Serif"
      FontSize        =   8.25
      FontStrikethru  =   0   'False
      FontUnderline   =   0   'False
      Height          =   255
      Left            =   1140
      TabIndex        =   3
      Top             =   2880
      Width           =   2535
   End
   Begin CommandButton BtnAcaba 
      Caption         =   "Aceptar "
      Default         =   -1  'True
      Height          =   375
      Left            =   60
      TabIndex        =   4
      Top             =   3360
      Width           =   1455
   End
   Begin CommandButton BtnCancel 
      Caption         =   "Cancelar"
      Height          =   375
      Left            =   1680
      TabIndex        =   5
      Top             =   3360
      Width           =   1455
   End
   Begin CommonDialog cmd 
      Filter          =   "Imaxes (*.bmp, .ico)|*.bmp;*.ico"
      Left            =   4140
      Top             =   2220
   End
   Begin SSCommand Img 
      BevelWidth      =   0
      Enabled         =   0   'False
      Font3D          =   0  'None
      Height          =   1125
      Left            =   1140
      TabIndex        =   0
      Top             =   1740
      Width           =   2535
   End
   Begin Label lbCodi 
      BackStyle       =   0  'Transparent
      FontBold        =   0   'False
      FontItalic      =   0   'False
      FontName        =   "MS Sans Serif"
      FontSize        =   8.25
      FontStrikethru  =   0   'False
      FontUnderline   =   0   'False
      ForeColor       =   &H00FF0000&
      Height          =   195
      Left            =   180
      TabIndex        =   8
      Top             =   720
      Width           =   1875
   End
   Begin Label LbName 
      BackStyle       =   0  'Transparent
      Caption         =   "Nome do grupo:"
      Height          =   255
      Left            =   180
      TabIndex        =   7
      Top             =   120
      Width           =   4335
   End
   Begin Label Label2 
      BackStyle       =   0  'Transparent
      Caption         =   "Imaxe:"
      Height          =   255
      Left            =   180
      TabIndex        =   6
      Top             =   1020
      Width           =   4395
   End
End
Option Explicit

Sub BtnAcaba_Click ()

Dim u As Dynaset
Dim ug As String
Dim s As String
Dim msg As String

If editGroup Then ug = "group" Else ug = "user"

msg = ""
If itemName = "" Then
  msg = "¡Non se pode deixa-lo nome en branco!"
Else
  If InStr(itemName, Chr(34)) Then
    msg = "Non se poden utilizar comiñas nos nomes"
  End If
End If

If msg <> "" Then
  MsgBox msg, 48
  Exit Sub
End If

If editGroup Then
  Set u = db.CreateDynaset("groups")
Else
  Set u = db.CreateDynaset("SELECT * FROM users WHERE groupId=" & editGroupId)
End If

s = ug & "Name=" & Chr(34) & itemName & Chr(34)
u.FindFirst s
Do Until u.NoMatch
 If u(ug & "Id") <> editItem Then
   If editGroup Then
     msg = "Xa hai outro grupo con ese mesmo nome"
   Else
     msg = "¡ Xa hai outro usuario con ese mesmo nome no grupo!"
   End If
 End If
 u.FindNext s
Loop

If msg <> "" Then
  u.Close
  MsgBox msg, 48
  Exit Sub
End If

If editItem = 0 Then
  u.AddNew
Else
  u.FindFirst ug & "Id=" & editItem
  u.Edit
End If

u(ug & "Name") = itemName
u(ug & "Img") = itemImg
If editGroup = False Then
  u("groupId") = editGroupId
End If
u.Update
u.Close
lastName = itemName

Unload EditUsr

End Sub

Sub BtnAjuda_Click ()

SendKeys "{F1}"

End Sub

Sub BtnCancel_Click ()

cancelEdit = True
Unload EditUsr

End Sub

Sub BtnCercaImg_Click ()

Dim i As Integer
Dim s As String

cmd.Filename = itemImg
If itemImg = "" Then
  s = reportBase
Else
  s = itemImg
End If
For i = Len(s) To 1 Step -1
  If Mid(s, i, 1) = "\" Then Exit For
Next i

cmd.InitDir = Left(s, i - 1)
cmd.Action = 1

itemImg = cmd.Filename
showImage

End Sub

Sub Form_Load ()

Dim u As Dynaset
Dim s As String
Dim v As Variant

EditUsr.Icon = ClicDB.Icon
CentrarImatge EditUsr

cancelEdit = False
lastName = ""

img.Picture = LoadPicture()
img.Caption = ""

If editGroup Then
  EditUsr.Caption = "Propiedades do grupo"
  LbName = "Nome do grupo:"
Else
  EditUsr.Caption = "Propiedades do usuario"
  LbName = "Nome do usuario:"
End If
 
If editItem <> 0 Then
  lbCodi = "Código: " & editItem
  If editGroup Then s = "groups" Else s = "users"
  Set u = db.CreateDynaset(s)
  If editGroup Then s = "groupId=" Else s = "userId="
  u.FindFirst s & editItem
  If u.NoMatch Then
    MsgBox "¡Non se pode localizar este grupo ou usuario!"
    u.Close
    cancelEdit = True
    Unload EditUsr
    Exit Sub
  Else
    If editGroup Then s = "groupName" Else s = "userName"
    itemName = u(s)
    If editGroup Then s = "groupImg" Else s = "userImg"
    v = u(s)
    If Not IsNull(v) Then itemImg = v
  End If
  u.Close
Else
  lbCodi = ""
End If

showImage

End Sub

Sub showImage ()

If itemImg = "" Then
  img.Picture = LoadPicture()
Else
  On Error GoTo dtgrError
  img.Picture = LoadPicture(itemImg)
  On Error GoTo 0
End If
 
Exit Sub

dtgrError:
 img.Caption = "ERRO"
 Resume Next

End Sub

