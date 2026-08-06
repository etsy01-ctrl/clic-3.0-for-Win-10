VERSION 2.00
Begin Form InfoPac 
   BackColor       =   &H00C0C0C0&
   BorderStyle     =   3  'Fixed Double
   Caption         =   "Informes por paquetes"
   ClientHeight    =   6420
   ClientLeft      =   1470
   ClientTop       =   1740
   ClientWidth     =   9525
   ControlBox      =   0   'False
   Height          =   6825
   HelpContextID   =   700
   Icon            =   0
   Left            =   1410
   LinkTopic       =   "Form1"
   MaxButton       =   0   'False
   MinButton       =   0   'False
   ScaleHeight     =   6420
   ScaleWidth      =   9525
   Top             =   1395
   Width           =   9645
   Begin CommandButton BtnAjuda 
      Caption         =   "Axuda"
      Height          =   315
      Left            =   7320
      TabIndex        =   31
      Top             =   6060
      Width           =   2115
   End
   Begin CommandButton BtnPrint 
      Caption         =   "Imprimir..."
      Height          =   315
      Left            =   7320
      TabIndex        =   29
      Top             =   5700
      Width           =   2115
   End
   Begin CommandButton BtnDates 
      Caption         =   "Periodo"
      FontBold        =   0   'False
      FontItalic      =   0   'False
      FontName        =   "MS Sans Serif"
      FontSize        =   8.25
      FontStrikethru  =   0   'False
      FontUnderline   =   0   'False
      Height          =   315
      Left            =   6600
      TabIndex        =   28
      Top             =   120
      Width           =   2835
   End
   Begin ComboBox ComboPacs 
      Height          =   315
      Left            =   1140
      Style           =   2  'Dropdown List
      TabIndex        =   27
      Top             =   120
      Width           =   2115
   End
   Begin Grid UsersGrid 
      Cols            =   4
      FixedCols       =   0
      FixedRows       =   0
      FontBold        =   0   'False
      FontItalic      =   0   'False
      FontName        =   "MS Sans Serif"
      FontSize        =   8.25
      FontStrikethru  =   0   'False
      FontUnderline   =   0   'False
      Height          =   2415
      HighLight       =   0   'False
      Left            =   3120
      TabIndex        =   26
      Top             =   3900
      Width           =   4095
   End
   Begin Grid SesGrid 
      Enabled         =   0   'False
      FixedCols       =   0
      FixedRows       =   0
      FontBold        =   0   'False
      FontItalic      =   0   'False
      FontName        =   "MS Sans Serif"
      FontSize        =   8.25
      FontStrikethru  =   0   'False
      FontUnderline   =   0   'False
      Height          =   975
      HighLight       =   0   'False
      Left            =   7320
      Rows            =   4
      ScrollBars      =   0  'None
      TabIndex        =   23
      Top             =   4200
      Width           =   2115
   End
   Begin GRAPH gr 
      AsciiFFamily    =   "2~2~2~2"
      AutoInc         =   0  'Off
      Background      =   3  'Cyan
      GraphType       =   6  'Line
      GridStyle       =   3  'Horizontal and Vertical
      Height          =   2295
      Left            =   120
      NumPoints       =   2
      NumSets         =   2
      TabIndex        =   22
      Top             =   1260
      Width           =   6795
      YAxisMax        =   100
      YAxisStyle      =   2  'User-defined
      YAxisTicks      =   4
   End
   Begin PictureBox Picture1 
      BackColor       =   &H00808000&
      Height          =   375
      Left            =   120
      ScaleHeight     =   345
      ScaleWidth      =   6765
      TabIndex        =   18
      Top             =   900
      Width           =   6795
      Begin Shape Shape6 
         FillColor       =   &H00FF0000&
         FillStyle       =   0  'Solid
         Height          =   135
         Left            =   5040
         Top             =   60
         Width           =   120
      End
      Begin Shape Shape5 
         FillColor       =   &H0000FF00&
         FillStyle       =   0  'Solid
         Height          =   135
         Left            =   3480
         Top             =   60
         Width           =   135
      End
      Begin Label Label11 
         BackStyle       =   0  'Transparent
         Caption         =   "Actividades resoltas"
         FontBold        =   0   'False
         FontItalic      =   0   'False
         FontName        =   "MS Sans Serif"
         FontSize        =   8.25
         FontStrikethru  =   0   'False
         FontUnderline   =   0   'False
         ForeColor       =   &H00FFFFFF&
         Height          =   255
         Left            =   5280
         TabIndex        =   21
         Top             =   60
         Width           =   1365
      End
      Begin Label Label10 
         BackStyle       =   0  'Transparent
         Caption         =   "Precisión global"
         FontBold        =   0   'False
         FontItalic      =   0   'False
         FontName        =   "MS Sans Serif"
         FontSize        =   8.25
         FontStrikethru  =   0   'False
         FontUnderline   =   0   'False
         ForeColor       =   &H00FFFFFF&
         Height          =   210
         Left            =   3720
         TabIndex        =   20
         Top             =   60
         Width           =   1275
      End
      Begin Label Label9 
         BackStyle       =   0  'Transparent
         Caption         =   "Evolución do paquete"
         ForeColor       =   &H00FFFFFF&
         Height          =   255
         Left            =   60
         TabIndex        =   19
         Top             =   60
         Width           =   2175
      End
   End
   Begin ListBox SesList 
      FontBold        =   0   'False
      FontItalic      =   0   'False
      FontName        =   "MS Sans Serif"
      FontSize        =   8.25
      FontStrikethru  =   0   'False
      FontUnderline   =   0   'False
      Height          =   1200
      Left            =   7320
      TabIndex        =   17
      Top             =   2700
      Width           =   1455
   End
   Begin ComboBox ComboActs 
      Height          =   315
      Left            =   1140
      Style           =   2  'Dropdown List
      TabIndex        =   14
      Top             =   480
      Width           =   2115
   End
   Begin PictureBox PbGraf 
      BackColor       =   &H00808000&
      Height          =   375
      Left            =   120
      ScaleHeight     =   345
      ScaleWidth      =   2865
      TabIndex        =   8
      Top             =   3600
      Width           =   2895
      Begin Label Label6 
         BackStyle       =   0  'Transparent
         Caption         =   "Distribución de resultados"
         ForeColor       =   &H00FFFFFF&
         Height          =   255
         Left            =   60
         TabIndex        =   11
         Top             =   60
         Width           =   2475
      End
   End
   Begin GRAPH grDist 
      AsciiFFamily    =   "1~2~2~2"
      AutoInc         =   0  'Off
      Background      =   3  'Cyan
      GraphStyle      =   2
      GridStyle       =   3  'Horizontal and Vertical
      Height          =   2355
      Left            =   120
      LegendStyle     =   1  'Color
      TabIndex        =   7
      Top             =   3960
      Width           =   2895
   End
   Begin ListBox ListUsers 
      Height          =   420
      Left            =   4200
      TabIndex        =   6
      Top             =   5820
      Visible         =   0   'False
      Width           =   510
   End
   Begin ComboBox ComboUsers 
      ForeColor       =   &H000000FF&
      Height          =   315
      Left            =   4200
      Style           =   2  'Dropdown List
      TabIndex        =   5
      Top             =   480
      Width           =   2235
   End
   Begin ListBox ListGroups 
      Height          =   420
      Left            =   3480
      TabIndex        =   4
      Top             =   5760
      Visible         =   0   'False
      Width           =   510
   End
   Begin ComboBox ComboGroups 
      ForeColor       =   &H00FF0000&
      Height          =   315
      Left            =   4200
      Style           =   2  'Dropdown List
      TabIndex        =   3
      Top             =   120
      Width           =   2235
   End
   Begin Grid DataGrid 
      Enabled         =   0   'False
      FixedCols       =   0
      FixedRows       =   0
      FontBold        =   0   'False
      FontItalic      =   0   'False
      FontName        =   "MS Sans Serif"
      FontSize        =   8.25
      FontStrikethru  =   0   'False
      FontUnderline   =   0   'False
      Height          =   1455
      HighLight       =   0   'False
      Left            =   7080
      Rows            =   6
      ScrollBars      =   0  'None
      TabIndex        =   1
      Top             =   900
      Width           =   2355
   End
   Begin CommandButton BtnAcaba 
      Caption         =   "Aceptar "
      Default         =   -1  'True
      Height          =   315
      Left            =   7320
      TabIndex        =   0
      Top             =   5340
      Width           =   2115
   End
   Begin ListBox SesDtList 
      Height          =   420
      Left            =   4800
      TabIndex        =   24
      Top             =   5820
      Visible         =   0   'False
      Width           =   555
   End
   Begin Label LbDates 
      Alignment       =   2  'Center
      BackStyle       =   0  'Transparent
      Caption         =   "Periodo"
      FontBold        =   0   'False
      FontItalic      =   0   'False
      FontName        =   "MS Sans Serif"
      FontSize        =   8.25
      FontStrikethru  =   0   'False
      FontUnderline   =   0   'False
      Height          =   195
      Left            =   8460
      TabIndex        =   30
      Top             =   480
      Visible         =   0   'False
      Width           =   975
   End
   Begin Image Image1 
      Height          =   1170
      Left            =   8940
      Picture         =   INFOPAC.FRX:0000
      Top             =   2760
      Width           =   330
   End
   Begin Label Label16 
      BackStyle       =   0  'Transparent
      Caption         =   "Datos da sesión"
      Height          =   285
      Left            =   7320
      TabIndex        =   2
      Top             =   3960
      Width           =   2115
   End
   Begin Label Label15 
      BackStyle       =   0  'Transparent
      Caption         =   "Sesións:"
      Height          =   285
      Left            =   7320
      TabIndex        =   9
      Top             =   2460
      Width           =   1455
   End
   Begin Label Label14 
      BackStyle       =   0  'Transparent
      Caption         =   "Clasificación de usuarios:"
      Height          =   285
      Left            =   3120
      TabIndex        =   10
      Top             =   3660
      Width           =   4095
   End
   Begin Label Label1 
      BackStyle       =   0  'Transparent
      Caption         =   "Datos globais:"
      Height          =   285
      Left            =   7080
      TabIndex        =   12
      Top             =   660
      Width           =   2355
   End
   Begin Label Label13 
      BackStyle       =   0  'Transparent
      Caption         =   "Usuario:"
      Height          =   285
      Left            =   3360
      TabIndex        =   13
      Top             =   540
      Width           =   855
   End
   Begin Label Label12 
      BackStyle       =   0  'Transparent
      Caption         =   "Grupo:"
      Height          =   285
      Left            =   3360
      TabIndex        =   25
      Top             =   180
      Width           =   855
   End
   Begin Label Label8 
      BackStyle       =   0  'Transparent
      Caption         =   "Paquete:"
      Height          =   285
      Left            =   120
      TabIndex        =   16
      Top             =   180
      Width           =   975
   End
   Begin Label Label7 
      BackStyle       =   0  'Transparent
      Caption         =   "Actividade:"
      Height          =   285
      Left            =   120
      TabIndex        =   15
      Top             =   540
      Width           =   975
   End
End
Option Explicit
Dim noCalc As Integer
Dim pacName As String

Sub BtnAcaba_Click ()

Unload InfoPac

End Sub

Sub BtnAjuda_Click ()

SendKeys "{F1}"

End Sub

Sub BtnDates_Click ()

Dates.Show 1
BtnDates.Caption = "Dende " & Format(DesDe, dateFmt) & " ata " & Format(FinsA, dateFmt)
If cancelData = False Then
  screen.MousePointer = 11
  CalcData
  screen.MousePointer = 0
End If

End Sub

Sub BtnPrint_Click ()

Dim formHeight As Long
Dim extraHeight As Long

'InfoPac.Visible = False

poShSess = True
Load PrintOps
PrintOps.Show 1
If poOk = False Then Exit Sub

InfoPac.Refresh
screen.MousePointer = 11
If poScreen Then
  printScreen
Else
  printList
End If
screen.MousePointer = 0
InfoPac.Refresh

End Sub

Sub CalcData ()

 Dim u As Dynaset
 Dim q As QueryDef
 Dim f As String
 Dim s As String, g As String
 Dim gf As String, uf As String, af As String, pf As String
 Static dv(5) As Integer
 Dim i As Integer, j As Integer, k As Integer

 If noCalc Then Exit Sub

 ClearData

 If ComboActs.ListIndex = 0 Then
   af = "*"
 Else
   af = ComboActs.Text
 End If
 gf = ListGroups.List(ComboGroups.ListIndex)
 uf = ListUsers.List(ComboUsers.ListIndex)
 pf = Chr(34) & pacName & Chr(34)
 f = "pacName=" & pf
 pf = pacName
 
 DataGrid.Col = 1
 Set q = db.OpenQueryDef("globalPac")
 q!whatGroup = gf
 q!whatUser = uf
 q!whatAct = af
 q!from = DesDe
 q!to = FinsA + 1
 Set u = q.CreateDynaset()
 u.FindFirst f
 If Not u.NoMatch Then
  DataGrid.Row = 2: DataGrid.Text = u("numActs")
  DataGrid.Row = 3: DataGrid.Text = u("actsSolved") & " (" & CInt(CLng(u("actsSolved")) * 100 / u("numActs")) & "%)"
  DataGrid.Row = 4: DataGrid.Text = SecsToStr(u("totalTime"))
  DataGrid.Row = 5: DataGrid.Text = CInt(u("globalPrec")) & "%"
  grDist.AutoInc = False
  grDist.NumPoints = 5
  grDist.AsciiFFamily = "1~1~1~1"
  grDist.ThisSet = 1
  dv(1) = -Val(u("p1"))
  dv(2) = -Val(u("p2")) - dv(1)
  dv(3) = -Val(u("p3")) - dv(1) - dv(2)
  dv(4) = -Val(u("p4")) - dv(1) - dv(2) - dv(3)
  dv(5) = Val(u("numActs")) - dv(1) - dv(2) - dv(3) - dv(4)
  For i = 1 To 5
    grDist.ThisPoint = i
    grDist.GraphData = dv(i)
    grDist.LabelText = Str((i - 1) * 20) & "%"
  Next i
  grDist.DrawMode = 2
 Else
   grDist.GraphCaption = "SEN DATOS"
   grDist.DrawMode = 1
 End If
 u.Close
 q.Close

 i = 0
 Set q = db.OpenQueryDef("globalUser")
 q!whatAct = af
 q!whatPac = pf
 q!whatGroup = gf
 q!from = DesDe
 q!to = FinsA + 1
 Set u = q.CreateDynaset()
 Do Until u.EOF
   g = CInt(u("globalPrec")) & "%"
   g = g & Chr(9) & u("numActs")
   g = g & Chr(9) & u("userName")
   g = g & Chr(9) & u("groupName")
   i = i + 1
   If i > 1 Then
     UsersGrid.AddItem g
   Else
     SetGridText UsersGrid, 0, 1, 3, 1, g
   End If
   u.MoveNext
 Loop
 u.Close
 q.Close
 
 DataGrid.Row = 0: DataGrid.Text = i

 Set q = db.OpenQueryDef("groupedSes")
 q!whatGroup = gf
 q!whatUser = uf
 q!whatAct = af
 q!whatPac = pf
 q!from = DesDe
 q!to = FinsA + 1
 Set u = q.CreateDynaset()
 gr.AutoInc = False
 gr.NumPoints = 2
 gr.AsciiFFamily = "1~1~1~1"
 i = 0
 Do Until u.EOF
   i = i + 1
   If i > gr.NumPoints Then
     gr.NumPoints = gr.NumPoints + 1
   End If
   gr.ThisPoint = i
   gr.ThisSet = 1
   j = CInt(CLng(u("actsSolved")) * 100 / u("numActs"))
   gr.GraphData = j
   gr.ThisSet = 2
   k = u("globalPrec")
   gr.GraphData = k
   s = u("gDateTime")
   s = Mid(s, 9, 2) & "/" & Mid(s, 6, 2) & "/" & Left(s, 4)
   SesList.AddItem s
   g = u("numActs") & " " & u("actsSolved") & " " & u("totalTime") & " " & CInt(u("globalPrec"))
   SesDtList.AddItem g
   gr.LabelText = Left(s, 5)
   u.MoveNext
 Loop
 
 DataGrid.Row = 1: DataGrid.Text = i
 
 If i = 1 Then
   gr.ThisPoint = 2
   gr.ThisSet = 1
   gr.GraphData = j
   gr.ThisSet = 2
   gr.GraphData = k
 End If
 If i > 0 Then
   If i > 7 Then
     gr.LabelEvery = i / 7
   Else
     gr.LabelEvery = 1
   End If
   gr.DrawMode = 2
   SesList.ListIndex = SesList.ListCount - 1
 Else
   gr.GraphCaption = "SEN DATOS"
   gr.DrawMode = 1
 End If
 u.Close
 q.Close

End Sub

Sub ClearData ()

 Dim i As Integer

 SetGridText DataGrid, 1, 0, 1, 5, ""
 SesList.Clear
 SesDtList.Clear
 
 For i = 2 To UsersGrid.Rows - 1
   UsersGrid.RemoveItem 2
 Next i
 SetGridText UsersGrid, 0, 1, 3, 1, ""

 gr.DataReset = 9
 gr.GraphCaption = ""
 gr.DrawMode = 1
 
 grDist.DataReset = 9
 grDist.GraphCaption = ""
 grDist.DrawMode = 1

End Sub

Sub ComboActs_Click ()

screen.MousePointer = 11
noCalc = False
CalcData
screen.MousePointer = 0

End Sub

Sub ComboGroups_Click ()

Dim u As Dynaset
Dim f As String

screen.MousePointer = 11
ComboUsers.Clear
ListUsers.Clear
ComboUsers.AddItem "Tódolos usuarios"
ListUsers.AddItem "*"

f = "SELECT * FROM users"
If ComboGroups.ListIndex > 0 Then
 f = f & " WHERE (GroupId=" & ListGroups.List(ComboGroups.ListIndex) & ")"
End If
f = f & " ORDER BY userName"
Set u = db.CreateDynaset(f)
Do Until u.EOF
  ComboUsers.AddItem u("userName")
  ListUsers.AddItem u("userId")
  u.MoveNext
Loop
u.Close
ComboUsers.ListIndex = 0
screen.MousePointer = 0

End Sub

Sub ComboPacs_Click ()

Dim q As QueryDef
Dim u As Dynaset
Dim f As String

screen.MousePointer = 11
pacName = ComboPacs.List(ComboPacs.ListIndex)
ComboActs.Clear
ComboActs.AddItem "Tódalas actividades"
Set q = db.OpenQueryDef("listActs")
q!from = DesDe
q!to = FinsA + 1
Set u = q.CreateDynaset()
f = "pacName=" & Chr(34) & pacName & Chr(34)
u.FindFirst f
Do Until u.NoMatch
  ComboActs.AddItem u("act")
  u.FindNext f
Loop
u.Close
q.Close
ComboActs.ListIndex = 0
screen.MousePointer = 0

End Sub

Sub ComboUsers_Click ()

screen.MousePointer = 11
CalcData
screen.MousePointer = 0

End Sub

Sub Form_Load ()

Dim s As String
Dim u As Dynaset
Dim q As QueryDef
Dim l As Integer
Dim i As Integer

InfoPac.Icon = ClicDB.Icon
screen.MousePointer = 11

noCalc = True

CentrarImatge InfoPac
BtnDates.Caption = "Dende " & Format(DesDe, dateFmt) & " ata " & Format(FinsA, dateFmt)

DataGrid.ColWidth(1) = 700
DataGrid.ColWidth(0) = DataGrid.Width - DataGrid.ColWidth(1) - 50
s = "Usuarios" & Chr(13) & "Sessions" & Chr(13) & "Actividades feitas" & Chr(13) & "Actividades resoltas" & Chr(13) & "Tempo total" & Chr(13) & "Precisión global"
SetGridText DataGrid, 0, 0, 0, 5, s
DataGrid.FixedCols = 1
DataGrid.ColAlignment(1) = 1

SesGrid.ColWidth(1) = 700
SesGrid.ColWidth(0) = SesGrid.Width - SesGrid.ColWidth(1) - 50
s = "Actividades feitas" & Chr(13) & "Actividades resoltas" & Chr(13) & "Tempo total" & Chr(13) & "Precisión global"
SetGridText SesGrid, 0, 0, 0, 3, s
SesGrid.FixedCols = 1
SesGrid.ColAlignment(1) = 1

l = (UsersGrid.Width - 380) / 8
For i = 0 To 3
 If i < 2 Then
   UsersGrid.ColAlignment(i) = 1
   UsersGrid.ColWidth(i) = l
 Else
   UsersGrid.ColWidth(i) = 3 * l
 End If
Next i
s = "Prec." & Chr(9) & "Acts." & Chr(9) & "usuario" & Chr(9) & "Grupo"
SetGridText UsersGrid, 0, 0, 3, 0, s
UsersGrid.FixedRows = 1

Set q = db.OpenQueryDef("listPacs")
q!whatGroup = "*"
q!from = DesDe
q!to = FinsA + 1
Set u = q.CreateDynaset()
Do Until u.EOF
  ComboPacs.AddItem u("pacName")
  u.MoveNext
Loop
u.Close
q.Close
If ComboPacs.ListCount = 0 Then
  screen.MousePointer = 0
  MsgBox "¡Non hai ningunha actividade rexistrada na base de datos!"
  BtnAcaba_Click
  Exit Sub
End If

ComboPacs.ListIndex = 0

ComboGroups.AddItem "Tódolos grupos"
ListGroups.AddItem "*"
Set u = db.CreateDynaset("SELECT * FROM groups ORDER BY groupName")
Do Until u.EOF
  ComboGroups.AddItem u("groupName")
  ListGroups.AddItem u("groupId")
  u.MoveNext
Loop
u.Close
ComboGroups.ListIndex = 0

screen.MousePointer = 0

End Sub

Sub printList ()

Dim s As Integer, s0 As Integer

On Error GoTo errorPrintList

If initPrinter() = False Then Exit Sub

printer.FontBold = True
printer.Print mp & "Paquete: " & ComboPacs.List(ComboPacs.ListIndex)
printer.Print mp & "Actividade: " & ComboActs.List(ComboActs.ListIndex)
printer.Print mp & "Grupo: " & ComboGroups.List(ComboGroups.ListIndex)
printer.Print mp & "Usuario: " & ComboUsers.List(ComboUsers.ListIndex)
printer.Print mp & "Periodo: " & Format(DesDe, "dd/mm/yy") & " - " & Format(FinsA, "dd/mm/yy")

printer.Print

If printGrid(DataGrid, True, True, "DATOS GLOBAIS", False) = False Then Exit Sub

If poAllSessions Then
  s0 = SesList.ListIndex
  For s = 0 To SesList.ListCount - 1
    SesList.ListIndex = s
    printer.Print
    If printGrid(SesGrid, False, False, "SESIÓN " & SesList.List(s), False) = False Then Exit Sub
  Next s
  SesList.ListIndex = s0
Else
  printer.Print
  If printGrid(SesGrid, False, False, "SESIÓN " & SesList.List(SesList.ListIndex), False) = False Then Exit Sub
End If

printer.Print

If printGrid(UsersGrid, False, True, "CLASIFICACIÓN DE USUARIOS", True) = False Then Exit Sub

printer.EndDoc

Exit Sub

errorPrintList:
MsgBox prnErr, 48
Exit Sub


End Sub

Sub printScreen ()

Dim formHeight As Long
Dim extraHeight As Long

'InfoPac.Visible = False

formHeight = InfoPac.Height
extraHeight = UsersGrid.RowHeight(0) * UsersGrid.Rows - UsersGrid.Height
If extraHeight < 0 Then extraHeight = 0

InfoPac.Height = InfoPac.Height + extraHeight
UsersGrid.Height = UsersGrid.Height + extraHeight
LbDates.Move BtnDates.Left, BtnDates.Top, BtnDates.Width, BtnDates.Height
LbDates.Caption = BtnDates.Caption
LbDates.Visible = True
BtnDates.Visible = False
InfoPac.BackColor = &HFFFFFF
BtnAcaba.Visible = False
BtnPrint.Visible = False

On Error GoTo errprint
InfoPac.PrintForm
On Error GoTo 0

InfoPac.BackColor = &HC0C0C0
LbDates.Visible = False
BtnDates.Visible = True
UsersGrid.Height = UsersGrid.Height - extraHeight
InfoPac.Height = formHeight
BtnAcaba.Visible = True
BtnPrint.Visible = True

InfoPac.Refresh
'InfoPac.Visible = True

Exit Sub

errprint:
MsgBox prnErr, 48
Resume Next

End Sub

Sub SesList_Click ()

  Dim g As String
  Dim s As String
  Dim p As Integer
  Dim mk As Integer
  Dim sol As Integer

  screen.MousePointer = 11
  s = SesDtList.List(SesList.ListIndex)
  p = 1
  g = extractParam(s, p)
  mk = Val(g)
  sol = Val(extractParam(s, p))
  g = g & Chr(13) & sol & " (" & CInt(CLng(sol) * 100 / mk) & "%)"
  g = g & Chr(13) & SecsToStr(Val(extractParam(s, p)))
  g = g & Chr(13) & extractParam(s, p) & "%"
  SetGridText SesGrid, 1, 0, 1, 3, g
  screen.MousePointer = 0

End Sub

