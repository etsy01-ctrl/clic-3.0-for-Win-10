VERSION 2.00
Begin Form InfoGrp 
   BackColor       =   &H00C0C0C0&
   BorderStyle     =   3  'Fixed Double
   Caption         =   "Informean taldeka"
   ClientHeight    =   6420
   ClientLeft      =   1275
   ClientTop       =   1740
   ClientWidth     =   9525
   ControlBox      =   0   'False
   Height          =   6825
   HelpContextID   =   500
   Icon            =   0
   Left            =   1215
   MaxButton       =   0   'False
   MinButton       =   0   'False
   ScaleHeight     =   6420
   ScaleWidth      =   9525
   Top             =   1395
   Width           =   9645
   Begin CommandButton BtnAjuda 
      Caption         =   "Laguntza"
      Height          =   375
      Left            =   5760
      TabIndex        =   17
      Top             =   5880
      Width           =   1665
   End
   Begin CommandButton BtnDates 
      Caption         =   "Denboraldia"
      FontBold        =   0   'False
      FontItalic      =   0   'False
      FontName        =   "MS Sans Serif"
      FontSize        =   8.25
      FontStrikethru  =   0   'False
      FontUnderline   =   0   'False
      Height          =   315
      Left            =   5640
      TabIndex        =   15
      Top             =   120
      Width           =   3675
   End
   Begin ListBox ListGroups 
      Height          =   225
      Left            =   8100
      TabIndex        =   13
      Top             =   5880
      Visible         =   0   'False
      Width           =   855
   End
   Begin ComboBox ComboGroups 
      ForeColor       =   &H00FF0000&
      Height          =   315
      Left            =   960
      Style           =   2  'Dropdown List
      TabIndex        =   12
      Top             =   120
      Width           =   3930
   End
   Begin PictureBox PbGraf 
      BackColor       =   &H00808000&
      Height          =   375
      Left            =   120
      ScaleHeight     =   345
      ScaleWidth      =   6885
      TabIndex        =   9
      Top             =   600
      Width           =   6915
      Begin Label Label6 
         BackStyle       =   0  'Transparent
         Caption         =   "Taldearen eboluzioa"
         ForeColor       =   &H00FFFFFF&
         Height          =   255
         Left            =   120
         TabIndex        =   6
         Top             =   60
         Width           =   2175
      End
      Begin Label Label5 
         BackStyle       =   0  'Transparent
         Caption         =   "Zehaztapen orokorra"
         FontBold        =   0   'False
         FontItalic      =   0   'False
         FontName        =   "MS Sans Serif"
         FontSize        =   8.25
         FontStrikethru  =   0   'False
         FontUnderline   =   0   'False
         ForeColor       =   &H00FFFFFF&
         Height          =   210
         Left            =   3900
         TabIndex        =   11
         Top             =   60
         Width           =   1095
      End
      Begin Label Label4 
         BackStyle       =   0  'Transparent
         Caption         =   "Burututako ariketak"
         FontBold        =   0   'False
         FontItalic      =   0   'False
         FontName        =   "MS Sans Serif"
         FontSize        =   8.25
         FontStrikethru  =   0   'False
         FontUnderline   =   0   'False
         ForeColor       =   &H00FFFFFF&
         Height          =   255
         Left            =   5280
         TabIndex        =   10
         Top             =   60
         Width           =   1545
      End
      Begin Shape Shape4 
         FillColor       =   &H0000FF00&
         FillStyle       =   0  'Solid
         Height          =   135
         Left            =   3660
         Top             =   60
         Width           =   135
      End
      Begin Shape Shape3 
         FillColor       =   &H00FF0000&
         FillStyle       =   0  'Solid
         Height          =   135
         Left            =   5040
         Top             =   60
         Width           =   120
      End
   End
   Begin Grid SesGrid 
      Cols            =   5
      FixedCols       =   0
      FixedRows       =   0
      FontBold        =   0   'False
      FontItalic      =   0   'False
      FontName        =   "MS Sans Serif"
      FontSize        =   8.25
      FontStrikethru  =   0   'False
      FontUnderline   =   0   'False
      Height          =   2235
      HighLight       =   0   'False
      Left            =   120
      ScrollBars      =   2  'Vertical
      TabIndex        =   8
      Top             =   3480
      Width           =   9270
   End
   Begin CommandButton BtnPrint 
      Caption         =   "Inprimatu..."
      Height          =   375
      Left            =   3780
      TabIndex        =   5
      Top             =   5880
      Width           =   1665
   End
   Begin ComboBox ComboPacs 
      BackColor       =   &H00FFFFFF&
      ForeColor       =   &H00000000&
      Height          =   315
      Left            =   7200
      Style           =   2  'Dropdown List
      TabIndex        =   3
      Top             =   840
      Width           =   2130
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
      TabIndex        =   2
      Top             =   960
      Width           =   6915
      YAxisMax        =   100
      YAxisStyle      =   2  'User-defined
      YAxisTicks      =   4
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
      Left            =   7200
      Rows            =   6
      ScrollBars      =   0  'None
      TabIndex        =   1
      Top             =   1800
      Width           =   2115
   End
   Begin CommandButton BtnAcaba 
      Caption         =   "Ados"
      Default         =   -1  'True
      Height          =   375
      Left            =   1860
      TabIndex        =   0
      Top             =   5880
      Width           =   1575
   End
   Begin Label LbDates 
      Alignment       =   2  'Center
      BackStyle       =   0  'Transparent
      Caption         =   "Denboraldia"
      FontBold        =   0   'False
      FontItalic      =   0   'False
      FontName        =   "MS Sans Serif"
      FontSize        =   8.25
      FontStrikethru  =   0   'False
      FontUnderline   =   0   'False
      Height          =   195
      Left            =   7980
      TabIndex        =   16
      Top             =   6120
      Visible         =   0   'False
      Width           =   975
   End
   Begin Label Label2 
      BackStyle       =   0  'Transparent
      Caption         =   "Taldea:"
      Height          =   285
      Left            =   120
      TabIndex        =   14
      Top             =   180
      Width           =   855
   End
   Begin Label Label7 
      BackStyle       =   0  'Transparent
      Caption         =   "Laburpen orokorra:"
      Height          =   195
      Left            =   7200
      TabIndex        =   7
      Top             =   1560
      Width           =   2055
   End
   Begin Label Label1 
      BackStyle       =   0  'Transparent
      Caption         =   "Honi buruzko datuak:"
      Height          =   285
      Left            =   7200
      TabIndex        =   4
      Top             =   600
      Width           =   2295
   End
End
Option Explicit
Dim groupId As Long

Sub BtnAcaba_Click ()

Unload InfoGrp

End Sub

Sub BtnAjuda_Click ()

SendKeys "{F1}"

End Sub

Sub BtnDates_Click ()

Dates.Show 1
BtnDates.Caption = "Hemendik " & Format(DesDe, dateFmt) & " honera " & Format(FinsA, dateFmt)
If cancelData = False Then
  screen.MousePointer = 11
  CalcData
  screen.MousePointer = 0
End If

End Sub

Sub BtnPrint_Click ()

Dim extraHeight As Long
Dim formHeight As Long

'InfoGrp.Visible = False

poShSess = False
Load PrintOps
PrintOps.Show 1
If poOk = False Then Exit Sub

InfoGrp.Refresh
screen.MousePointer = 11
If poScreen Then
  printScreen
Else
  printList
End If
screen.MousePointer = 0
InfoGrp.Refresh

End Sub

Sub CalcData ()

 Dim u As Dynaset
 Dim q As Querydef
 Dim s As String
 Dim wp As String
 Dim f As String
 Dim g As String
 Dim i As Integer
 Dim j As Integer
 Dim k As Integer

 ClearData

 If ComboPacs.ListIndex = 0 Then
   wp = "*"
 Else
   wp = ComboPacs.Text
 End If
 f = "groupId=" & groupId

 DataGrid.Col = 1
 Set q = db.OpenQueryDef("countSesGrp")
 q!whatPac = wp
 q!from = DesDe
 q!to = FinsA + 1
 Set u = q.CreateDynaset()
 u.FindFirst f
 If Not u.NoMatch Then
  DataGrid.Row = 0: DataGrid.Text = u("numSessions")
  DataGrid.Row = 1: DataGrid.Text = u("numPacs")
 End If
 u.Close
 q.Close
 
 Set q = db.OpenQueryDef("globalGroup")
 q!whatPac = wp
 q!from = DesDe
 q!to = FinsA + 1
 Set u = q.CreateDynaset()
 u.FindFirst f
 If Not u.NoMatch Then
  DataGrid.Row = 2: DataGrid.Text = u("numActs")
  DataGrid.Row = 3: DataGrid.Text = u("actsSolved") & " (" & CInt(CLng(u("actsSolved")) * 100 / u("numActs")) & "%)"
  DataGrid.Row = 4: DataGrid.Text = secsToStr(u("totalTime"))
  DataGrid.Row = 5: DataGrid.Text = CInt(u("globalPrec")) & "%"
 End If
 u.Close
 q.Close

 Set q = db.OpenQueryDef("groupedSes")
 q!whatPac = wp
 q!whatUser = "*"
 q!whatAct = "*"
 q!whatGroup = groupId
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
   g = Mid(s, 9, 2) & "/" & Mid(s, 6, 2) & "/" & Left(s, 4)
   gr.LabelText = Left(g, 5)
   g = g & Chr(9) & u("numActs")
   g = g & Chr(9) & u("actsSolved") & " (" & Format(j, "00") & "%)"
   g = g & Chr(9) & secsToStr(u("totalTime"))
   g = g & Chr(9) & k & "%"
   If i > 1 Then
     SesGrid.AddItem g
   Else
     SetGridText SesGrid, 0, 1, 4, 1, g
   End If
   u.MoveNext
 Loop
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
 Else
   gr.GraphCaption = "DATURIK EZ!"
   gr.DrawMode = 1
 End If
 
 u.Close
 q.Close
 
End Sub

Sub ClearData ()
 
 Dim i As Integer

 SetGridText DataGrid, 1, 0, 1, 5, ""
 
 For i = 2 To SesGrid.Rows - 1
   SesGrid.RemoveItem 2
 Next i
 SetGridText SesGrid, 0, 1, 4, 1, ""
 
 gr.DataReset = 9
 gr.GraphCaption = ""
 gr.DrawMode = 1
 
End Sub

Sub ComboGroups_Click ()
 
 Dim q As Querydef
 Dim u As Dynaset
 Dim f As String

 groupId = Val(ListGroups.List(ComboGroups.ListIndex))
 If groupId = 0 Then Exit Sub

 screen.MousePointer = 11
 
 ComboPacs.Clear
 ComboPacs.AddItem "Pakete guztiak"
 ComboPacs.ListIndex = 0
 
 If groupId = 0 Then Exit Sub
 
 Set q = db.OpenQueryDef("pacsByGroup")
 q!from = DesDe
 q!to = FinsA + 1
 Set u = q.CreateDynaset()
 f = "groupId=" & groupId
 u.FindFirst f
 Do Until u.NoMatch
   ComboPacs.AddItem u("pacName")
   u.FindNext f
 Loop
 u.Close
 q.Close
 
 screen.MousePointer = 0
 
End Sub

Sub ComboPacs_Click ()

screen.MousePointer = 11
CalcData
screen.MousePointer = 0

End Sub

Sub Form_Load ()

Dim s As String
Dim i As Integer
Dim l As Integer
Dim u As Dynaset

InfoGrp.Icon = ClicDB.Icon
CentrarImatge InfoGrp
BtnDates.Caption = "Hemendik " & Format(DesDe, dateFmt) & " honera " & Format(FinsA, dateFmt)

DataGrid.ColWidth(1) = 700
DataGrid.ColWidth(0) = DataGrid.Width - DataGrid.ColWidth(1) - 50
s = "Sessions" & Chr(13) & "Paketeak" & Chr(13) & "Egindako ariketak" & Chr(13) & "Burututako ariketak" & Chr(13) & "Erabateko denbora" & Chr(13) & "Zehaztapen orokorra"
SetGridText DataGrid, 0, 0, 0, 5, s
DataGrid.FixedCols = 1
DataGrid.ColAlignment(1) = 1

l = (SesGrid.Width - 380) / 5
For i = 0 To 4
 SesGrid.ColAlignment(i) = 1
 SesGrid.ColWidth(i) = l
Next i

s = "Data" & Chr(9) & "Egindako ariketak" & Chr(9) & "Burututako ariketak" & Chr(9) & "Erabateko denbora" & Chr(9) & "Zehaztapen orokorra"
SetGridText SesGrid, 0, 0, 4, 0, s
SesGrid.FixedRows = 1

Set u = db.CreateDynaset("SELECT * FROM groups ORDER BY groupName")
Do Until u.EOF
  ComboGroups.AddItem u("groupName")
  ListGroups.AddItem u("groupId")
  u.MoveNext
Loop
u.Close
ComboGroups.ListIndex = 0

End Sub

Sub printList ()

On Error GoTo errorPrintList

If initPrinter() = False Then Exit Sub

printer.FontBold = True
printer.Print mp & "Taldea: " & ComboGroups.List(ComboGroups.ListIndex)
printer.Print mp & "Honi buruzko datuak: " & ComboPacs.List(ComboPacs.ListIndex)
printer.Print mp & "Denboraldia: " & Format(DesDe, "dd/mm/yy") & " - " & Format(FinsA, "dd/mm/yy")

printer.Print

If printGrid(DataGrid, True, True, "LABURPEN OROKORRA", False) = False Then Exit Sub

printer.Print

If printGrid(SesGrid, False, False, "SAIOEN XEHETASUNA", True) = False Then Exit Sub

printer.EndDoc

Exit Sub

errorPrintList:
MsgBox prnErr, 48
Exit Sub

End Sub

Sub printScreen ()

Dim extraHeight As Long
Dim formHeight As Long

'InfoGrp.Visible = False

formHeight = InfoGrp.Height
extraHeight = SesGrid.RowHeight(0) * SesGrid.Rows - SesGrid.Height
If extraHeight < 0 Then extraHeight = 0

InfoGrp.Height = InfoGrp.Height + extraHeight
SesGrid.Height = SesGrid.Height + extraHeight
LbDates.Move BtnDates.Left, BtnDates.Top, BtnDates.Width, BtnDates.Height
LbDates.Caption = BtnDates.Caption
LbDates.Visible = True
BtnDates.Visible = False
InfoGrp.BackColor = &HFFFFFF
BtnAcaba.Visible = False
BtnPrint.Visible = False

On Error GoTo errPrintScreen
InfoGrp.PrintForm
On Error GoTo 0

InfoGrp.BackColor = &HC0C0C0
SesGrid.Height = SesGrid.Height - extraHeight
InfoGrp.Height = formHeight
BtnAcaba.Visible = True
BtnPrint.Visible = True
LbDates.Visible = False
BtnDates.Visible = True

InfoGrp.Refresh
'InfoGrp.Visible = True

Exit Sub

errPrintScreen:
MsgBox prnErr, 48
Resume Next

End Sub

