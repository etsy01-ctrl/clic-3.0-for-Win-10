VERSION 2.00
Begin Form InfoUsr 
   BackColor       =   &H00C0C0C0&
   BorderStyle     =   3  'Fixed Double
   Caption         =   "Berichte für Benutzer"
   ClientHeight    =   6420
   ClientLeft      =   1245
   ClientTop       =   1275
   ClientWidth     =   9525
   ControlBox      =   0   'False
   Height          =   6825
   HelpContextID   =   600
   Icon            =   0
   Left            =   1185
   MaxButton       =   0   'False
   MinButton       =   0   'False
   ScaleHeight     =   6420
   ScaleWidth      =   9525
   Top             =   930
   Width           =   9645
   Begin CommandButton BtnAjuda 
      Caption         =   "Hilfe"
      Height          =   315
      Left            =   7500
      TabIndex        =   26
      Top             =   6000
      Width           =   1875
   End
   Begin CommandButton BtnDates 
      Caption         =   "Periode"
      FontBold        =   0   'False
      FontItalic      =   0   'False
      FontName        =   "MS Sans Serif"
      FontSize        =   8.25
      FontStrikethru  =   0   'False
      FontUnderline   =   0   'False
      Height          =   315
      Left            =   6600
      TabIndex        =   24
      Top             =   180
      Width           =   2835
   End
   Begin ListBox ListUsers 
      Height          =   615
      Left            =   5400
      TabIndex        =   21
      Top             =   5340
      Visible         =   0   'False
      Width           =   495
   End
   Begin ListBox ListGroups 
      Height          =   615
      Left            =   4860
      TabIndex        =   20
      Top             =   5340
      Visible         =   0   'False
      Width           =   495
   End
   Begin ComboBox ComboUsers 
      ForeColor       =   &H000000FF&
      Height          =   315
      Left            =   1620
      Style           =   2  'Dropdown List
      TabIndex        =   19
      Top             =   480
      Width           =   2595
   End
   Begin ComboBox ComboGroups 
      ForeColor       =   &H00FF0000&
      Height          =   315
      Left            =   1620
      Style           =   2  'Dropdown List
      TabIndex        =   18
      Top             =   120
      Width           =   2595
   End
   Begin PictureBox PbGraf 
      BackColor       =   &H00808000&
      Height          =   375
      Left            =   120
      ScaleHeight     =   345
      ScaleWidth      =   6825
      TabIndex        =   17
      Top             =   900
      Width           =   6855
      Begin Shape Shape2 
         FillColor       =   &H00FF0000&
         FillStyle       =   0  'Solid
         Height          =   135
         Left            =   5160
         Top             =   60
         Width           =   120
      End
      Begin Shape Shape1 
         FillColor       =   &H0000FF00&
         FillStyle       =   0  'Solid
         Height          =   135
         Left            =   3000
         Top             =   60
         Width           =   135
      End
      Begin Label Label3 
         BackStyle       =   0  'Transparent
         Caption         =   "richtige Übungen"
         FontBold        =   0   'False
         FontItalic      =   0   'False
         FontName        =   "MS Sans Serif"
         FontSize        =   8.25
         FontStrikethru  =   0   'False
         FontUnderline   =   0   'False
         ForeColor       =   &H00FFFFFF&
         Height          =   255
         Left            =   5400
         TabIndex        =   6
         Top             =   60
         Width           =   1365
      End
      Begin Label Label2 
         BackStyle       =   0  'Transparent
         Caption         =   "Trefferquote"
         FontBold        =   0   'False
         FontItalic      =   0   'False
         FontName        =   "MS Sans Serif"
         FontSize        =   8.25
         FontStrikethru  =   0   'False
         FontUnderline   =   0   'False
         ForeColor       =   &H00FFFFFF&
         Height          =   210
         Left            =   3240
         TabIndex        =   7
         Top             =   60
         Width           =   1875
      End
      Begin Label Label6 
         BackStyle       =   0  'Transparent
         Caption         =   "Entwicklung des Benutzers"
         ForeColor       =   &H00FFFFFF&
         Height          =   255
         Left            =   120
         TabIndex        =   15
         Top             =   60
         Width           =   3255
      End
   End
   Begin ListBox SesIdList 
      FontBold        =   0   'False
      FontItalic      =   0   'False
      FontName        =   "MS Sans Serif"
      FontSize        =   8.25
      FontStrikethru  =   0   'False
      FontUnderline   =   0   'False
      Height          =   225
      Left            =   4920
      TabIndex        =   13
      Top             =   6000
      Visible         =   0   'False
      Width           =   915
   End
   Begin ListBox SesList 
      FontBold        =   0   'False
      FontItalic      =   0   'False
      FontName        =   "MS Sans Serif"
      FontSize        =   8.25
      FontStrikethru  =   0   'False
      FontUnderline   =   0   'False
      Height          =   2565
      Left            =   120
      TabIndex        =   12
      Top             =   3720
      Width           =   1755
   End
   Begin Grid ActGrid 
      Cols            =   7
      FixedCols       =   0
      FixedRows       =   0
      FontBold        =   0   'False
      FontItalic      =   0   'False
      FontName        =   "MS Sans Serif"
      FontSize        =   8.25
      FontStrikethru  =   0   'False
      FontUnderline   =   0   'False
      Height          =   1215
      HighLight       =   0   'False
      Left            =   2340
      ScrollBars      =   2  'Vertical
      TabIndex        =   9
      Top             =   3720
      Width           =   7050
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
      Left            =   2340
      Rows            =   4
      ScrollBars      =   0  'None
      TabIndex        =   8
      Top             =   5280
      Width           =   2295
   End
   Begin CommandButton BtnPrint 
      Caption         =   "drucken..."
      Height          =   315
      Left            =   7500
      TabIndex        =   5
      Top             =   5580
      Width           =   1875
   End
   Begin ComboBox ComboPacs 
      Height          =   315
      Left            =   7140
      Style           =   2  'Dropdown List
      TabIndex        =   3
      Top             =   1140
      Width           =   2235
   End
   Begin GRAPH gr 
      AsciiFFamily    =   "2~2~2~2"
      AutoInc         =   0  'Off
      Background      =   3  'Cyan
      GraphType       =   6  'Line
      GridStyle       =   3  'Horizontal and Vertical
      Height          =   2115
      Left            =   120
      NumPoints       =   2
      NumSets         =   2
      PrintStyle      =   1  'Color
      TabIndex        =   2
      Top             =   1260
      Width           =   6855
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
      Height          =   1440
      HighLight       =   0   'False
      Left            =   7140
      Rows            =   6
      ScrollBars      =   0  'None
      TabIndex        =   1
      Top             =   1920
      Width           =   2235
   End
   Begin CommandButton BtnAcaba 
      Caption         =   "OK"
      Default         =   -1  'True
      Height          =   315
      Left            =   7500
      TabIndex        =   0
      Top             =   5160
      Width           =   1875
   End
   Begin Label LbDates 
      Alignment       =   2  'Center
      BackStyle       =   0  'Transparent
      Caption         =   "Periode"
      FontBold        =   0   'False
      FontItalic      =   0   'False
      FontName        =   "MS Sans Serif"
      FontSize        =   8.25
      FontStrikethru  =   0   'False
      FontUnderline   =   0   'False
      Height          =   195
      Left            =   8160
      TabIndex        =   25
      Top             =   600
      Visible         =   0   'False
      Width           =   975
   End
   Begin Label Label9 
      BackStyle       =   0  'Transparent
      Caption         =   "Benutzer:"
      Height          =   195
      Left            =   180
      TabIndex        =   23
      Top             =   540
      Width           =   855
   End
   Begin Label Label8 
      BackStyle       =   0  'Transparent
      Caption         =   "Benutzergruppe:"
      Height          =   195
      Left            =   180
      TabIndex        =   22
      Top             =   180
      Width           =   1455
   End
   Begin Image Image1 
      Height          =   135
      Left            =   1920
      Picture         =   INFOUSR.FRX:0000
      Top             =   3780
      Width           =   390
   End
   Begin Label Label7 
      BackStyle       =   0  'Transparent
      Caption         =   "globales Ergebnis:"
      Height          =   195
      Left            =   7140
      TabIndex        =   16
      Top             =   1680
      Width           =   2235
   End
   Begin Label LbResum 
      BackStyle       =   0  'Transparent
      Caption         =   "Ergebnis der Sitzung:"
      Height          =   195
      Left            =   2325
      TabIndex        =   14
      Top             =   5025
      Width           =   2310
   End
   Begin Label LbSes 
      BackStyle       =   0  'Transparent
      Caption         =   "Übungen"
      Height          =   195
      Left            =   2340
      TabIndex        =   11
      Top             =   3480
      Width           =   5445
   End
   Begin Label Label4 
      BackStyle       =   0  'Transparent
      Caption         =   "Sessions"
      Height          =   195
      Left            =   120
      TabIndex        =   10
      Top             =   3480
      Width           =   1680
   End
   Begin Label Label1 
      BackStyle       =   0  'Transparent
      Caption         =   "Dateien beziehen sich auf:"
      Height          =   195
      Left            =   7140
      TabIndex        =   4
      Top             =   900
      Width           =   2295
   End
End
Option Explicit

Dim groupId As Long
Dim userId As Long

Sub BtnAcaba_Click ()

Unload InfoUsr

End Sub

Sub BtnAjuda_Click ()

SendKeys "{F1}"

End Sub

Sub BtnDates_Click ()

Dates.Show 1
BtnDates.Caption = "von " & Format(DesDe, dateFmt) & " bis " & Format(FinsA, dateFmt)
If cancelData = False Then
  screen.MousePointer = 11
  CalcData
  screen.MousePointer = 0
End If


End Sub

Sub BtnPrint_Click ()

Dim extraHeight As Long
Dim formHeight As Long

'InfoUsr.Visible = False

poShSess = True
Load PrintOps
PrintOps.Show 1
If poOk = False Then Exit Sub

InfoUsr.Refresh

screen.MousePointer = 11
If poScreen Then
  printScreen
Else
  printList
End If
screen.MousePointer = 0

InfoUsr.Refresh

End Sub

Sub CalcData ()

 Dim u As DynaSet
 Dim q As QueryDef
 Dim s As String
 Dim wp As String
 Dim f As String
 Dim g As String
 Dim i As Integer
 Dim j As Integer
 Dim k As Integer

 ClearData
 If userId = 0 Then Exit Sub

 If ComboPacs.ListIndex = 0 Then
   wp = "*"
 Else
   wp = ComboPacs.Text
 End If
 f = "userId=" & userId

 DataGrid.Col = 1
 Set q = db.OpenQueryDef("countSes")
 q!whatPac = wp
 q!from = dts(DesDe)
 q!to = dts(FinsA + 1)
 Set u = q.CreateDynaset()
 u.FindFirst f
 If Not u.NoMatch Then
  DataGrid.Row = 0: DataGrid.Text = u("numSessions")
  DataGrid.Row = 1: DataGrid.Text = u("numPacs")
 End If
 u.Close
 q.Close
 
 Set q = db.OpenQueryDef("globalUser")
 q!whatPac = wp
 q!whatAct = "*"
 q!whatGroup = "*"
 q!from = dts(DesDe)
 q!to = dts(FinsA + 1)
 Set u = q.CreateDynaset()
 u.FindFirst f
 If Not u.NoMatch Then
  DataGrid.Row = 2: DataGrid.Text = u("numActs")
  DataGrid.Row = 3: DataGrid.Text = u("actsSolved") & " (" & CInt(CLng(u("actsSolved")) * 100 / u("numActs")) & "%)"
  DataGrid.Row = 4: DataGrid.Text = SecsToStr(u("totalTime"))
  DataGrid.Row = 5: DataGrid.Text = CInt(u("globalPrec")) & "%"
 End If
 u.Close
 q.Close

 Set q = db.OpenQueryDef("globalSes")
 q!whatPac = wp
 q!from = dts(DesDe)
 q!to = dts(FinsA + 1)
 Set u = q.CreateDynaset()
 gr.AutoInc = False
 gr.NumPoints = 2
 gr.AsciiFFamily = "1~1~1~1"
 i = 0
 u.FindFirst f
 Do Until u.NoMatch
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
   gr.LabelText = Format(u("dateTime"), "dd/mm")
   SesList.AddItem Format(u("dateTime"), "dd/mm/yyyy hh:nn")
   SesIdList.AddItem u("sessionId")
   u.FindNext f
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
   SesList.ListIndex = SesList.ListCount - 1
 Else
   gr.GraphCaption = "ohne Daten"
   gr.DrawMode = 1
 End If
 
 u.Close
 q.Close
 
End Sub

Sub clearAct ()
 
 Dim i As Integer

 For i = 2 To ActGrid.Rows - 1
   ActGrid.RemoveItem 2
 Next i
 SetGridText ActGrid, 0, 1, 6, 1, ""
 SetGridText SesGrid, 1, 0, 1, 3, ""
 lbSes = ""

End Sub

Sub ClearData ()
 
 Dim i As Integer

 SetGridText DataGrid, 1, 0, 1, 5, ""
 SesList.Clear
 SesIdList.Clear

 gr.DataReset = 9
 gr.GraphCaption = ""
 gr.DrawMode = 1
 
 clearAct

End Sub

Sub ComboGroups_Click ()

 Dim u As DynaSet
 Dim f As String
 Dim i As Integer, k As Integer

 screen.MousePointer = 11
 
 ComboUsers.Clear
 ListUsers.Clear
 If directToUser = 0 Then userId = 0
 groupId = ListGroups.List(ComboGroups.ListIndex)

 Set u = db.CreateDynaset("SELECT * FROM users WHERE (groupId=" & groupId & ") ORDER BY userName")
 If u.EOF Then
   ComboUsers.Enabled = False
   ComboPacs.Clear
   ClearData
   gr.GraphCaption = "ohne Daten"
   gr.DrawMode = 1
 Else
   k = 0: i = 0
   Do Until u.EOF
     If directToUser <> 0 Then
       If u("userId") = userId Then k = i
     End If
     ComboUsers.AddItem u("userName")
     ListUsers.AddItem u("userId")
     u.MoveNext
     i = i + 1
   Loop
   ComboUsers.ListIndex = k
 End If

 screen.MousePointer = 0

End Sub

Sub ComboPacs_Click ()

screen.MousePointer = 11
CalcData
screen.MousePointer = 0

End Sub

Sub ComboUsers_Click ()
 
 Dim q As QueryDef
 Dim u As DynaSet
 Dim f As String

 screen.MousePointer = 11
 
 userId = ListUsers.List(ComboUsers.ListIndex)
 ComboPacs.Clear
 ComboPacs.AddItem "alle Sammlungen"
 ComboPacs.ListIndex = 0

 If userId = 0 Then
   ComboUsers.Enabled = False
   ClearData
   screen.MousePointer = 0
   Exit Sub
 Else
   If directToUser = 0 Then ComboUsers.Enabled = True
 End If

 Set q = db.OpenQueryDef("pacsByUser")
 q!from = dts(DesDe)
 q!to = dts(FinsA + 1)
 Set u = q.CreateDynaset()
 f = "userId=" & userId
 u.FindFirst f
 Do Until u.NoMatch
   ComboPacs.AddItem u("pacName")
   u.FindNext f
 Loop
 u.Close
 q.Close
 
 'El calcData el fa el click al combo box
 'calcData
 
 screen.MousePointer = 0

End Sub

Sub FillActs (ByVal sgi As Integer)

Dim sessionId As Integer
Dim q As QueryDef
Dim u As DynaSet
Dim p As String
Dim f As String
Dim i As Integer
Dim g As String

clearAct
If sgi < 0 Or sgi > SesList.ListCount - 1 Then Exit Sub
 
sessionId = Val(SesIdList.List(sgi))
If sessionId = 0 Then Exit Sub

lbSes = "Übungen, die in der Sitzung bearbeitet wurden " & SesList.List(sgi)

If ComboPacs.ListIndex = 0 Then
  p = "*"
Else
  p = ComboPacs.Text
End If
f = "sessionId=" & sessionId

Set q = db.OpenQueryDef("actsByPac")
q!whatPac = p
Set u = q.CreateDynaset()
i = 0
u.FindFirst f
Do Until u.NoMatch
  i = i + 1
  g = u("pacName")
  g = g & Chr(9) & u("act")
  g = g & Chr(9) & SecsToStr(u("time"))
  g = g & Chr(9) & u("tries")
  g = g & Chr(9) & u("placed")
  If u("solved") Then
    g = g & Chr(9) & "X"
  Else
    g = g & Chr(9) & " "
  End If
  g = g & Chr(9) & u("precision") & "%"

  If i > 1 Then
     ActGrid.AddItem g
  Else
     SetGridText ActGrid, 0, 1, 6, 1, g
  End If
  u.FindNext f
Loop
u.Close
q.Close

Set q = db.OpenQueryDef("globalSes")
q!whatPac = p
q!from = dts(DesDe)
q!to = dts(FinsA + 1)
Set u = q.CreateDynaset()
u.FindFirst f
If Not u.NoMatch Then
  g = u("numActs")
  g = g & Chr(13) & u("actsSolved") & " (" & CInt(CLng(u("actsSolved")) * 100 / u("numActs")) & "%)"
  g = g & Chr(13) & SecsToStr(u("totalTime"))
  g = g & Chr(13) & CInt(u("globalPrec")) & "%"
  SetGridText SesGrid, 1, 0, 1, 3, g
End If
u.Close
q.Close

End Sub

Sub Form_Load ()

Dim u As DynaSet
Dim s As String
Dim i As Integer, k As Integer
Dim l1 As Integer, l2 As Integer

If directToUser <> 0 Then
  userId = directToUser
  Set u = db.CreateDynaset("users")
  s = "userId=" & userId
  u.FindFirst s
  If u.NoMatch Then
    u.Close
    MsgBox "FEHLER: Es wurde kein Benutzer in der Datenbank gefunden", 16
    Exit Sub
  End If
  groupId = u("groupId")
  u.Close
  ComboGroups.Enabled = False
  ComboUsers.Enabled = False
End If

InfoUsr.Icon = ClicDB.Icon
CentrarImatge InfoUsr
BtnDates.Caption = "von " & Format(DesDe, dateFmt) & " bis " & Format(FinsA, dateFmt)

DataGrid.ColWidth(1) = 700
DataGrid.ColWidth(0) = DataGrid.Width - DataGrid.ColWidth(1) - 50
s = "Sessions" & Chr(13) & "Sammlungen" & Chr(13) & "gemachte Übungen" & Chr(13) & "richtige Übungen" & Chr(13) & "gesamte Zeit" & Chr(13) & "Trefferquote"
SetGridText DataGrid, 0, 0, 0, 5, s
DataGrid.FixedCols = 1
DataGrid.ColAlignment(1) = 1

SesGrid.ColWidth(1) = 700
SesGrid.ColWidth(0) = SesGrid.Width - SesGrid.ColWidth(1) - 50
s = "gemachte Übungen" & Chr(13) & "richtige Übungen" & Chr(13) & "gesamte Zeit" & Chr(13) & "Trefferquote"
SetGridText SesGrid, 0, 0, 0, 3, s
SesGrid.FixedCols = 1
SesGrid.ColAlignment(1) = 1


l1 = 700
l2 = (ActGrid.Width - 380 - 5 * l1 - 200) / 2
For i = 0 To 6
 If i = 6 Then
   ActGrid.ColAlignment(i) = 1
   ActGrid.ColWidth(i) = l1 + 200
 ElseIf i > 1 Then
   ActGrid.ColAlignment(i) = 1
   ActGrid.ColWidth(i) = l1
 Else
   ActGrid.ColAlignment(i) = 0
   ActGrid.ColWidth(i) = l2
 End If
Next i
ActGrid.ColAlignment(5) = 2
s = "SAMMLUNG" & Chr(9) & "Übung" & Chr(9) & "Zeit" & Chr(9) & "Versuche" & Chr(9) & "Treffer" & Chr(9) & "Ergebnis" & Chr(9) & "Auswertung"
SetGridText ActGrid, 0, 0, 6, 0, s
ActGrid.FixedRows = 1

Set u = db.CreateDynaset("SELECT * FROM groups ORDER BY groupName")
k = 0: i = 0
Do Until u.EOF
  If directToUser <> 0 Then
    If groupId = u("groupId") Then k = i
  End If
  ComboGroups.AddItem u("groupName")
  ListGroups.AddItem u("groupId")
  u.MoveNext
  i = i + 1
Loop
u.Close
ComboGroups.ListIndex = k

End Sub

Sub printList ()

Dim s As Integer, s0 As Integer

On Error GoTo errorPrintList

If initPrinter() = False Then Exit Sub

printer.FontBold = True
printer.Print mp & "Benutzergruppe: " & ComboGroups.List(ComboGroups.ListIndex)
printer.Print mp & "Benutzer: " & ComboUsers.List(ComboUsers.ListIndex)
printer.Print mp & "Dateien beziehen sich auf: " & ComboPacs.List(ComboPacs.ListIndex)
printer.Print mp & "Periode: " & Format(DesDe, "dd/mm/yy") & " - " & Format(FinsA, "dd/mm/yy")

printer.Print

If printGrid(DataGrid, True, True, "Gesamtergebnis", False) = False Then Exit Sub

printer.Print

If poAllSessions Then
  s0 = SesList.ListIndex
  For s = 0 To SesList.ListCount - 1
    SesList.ListIndex = s
    printer.Print
    printer.FontBold = True
    printer.Print mp & "SITZUNG " & SesList.List(s)
    printer.FontBold = False
    printer.Print
    If printGrid(SesGrid, True, True, "Ergebnis der Sitzung:", False) = False Then Exit Sub
    printer.Print
    If printGrid(ActGrid, False, True, "Übungen:", True) = False Then Exit Sub
    printer.Print
  Next s
  SesList.ListIndex = s0
Else
  printer.Print
  printer.FontBold = True
  printer.Print mp & "SITZUNG " & SesList.List(SesList.ListIndex)
  printer.FontBold = False
  printer.Print
  If printGrid(SesGrid, True, True, "Ergebnis der Sitzung:", False) = False Then Exit Sub
  printer.Print
  If printGrid(ActGrid, False, True, "Übungen:", True) = False Then Exit Sub
End If

printer.Print

printer.EndDoc

Exit Sub

errorPrintList:
MsgBox prnErr, 48
Exit Sub


End Sub

Sub printScreen ()

Dim extraHeight As Long
Dim formHeight As Long

'InfoUsr.Visible = False

formHeight = InfoUsr.Height
extraHeight = ActGrid.RowHeight(0) * ActGrid.Rows - ActGrid.Height
If extraHeight < 0 Then extraHeight = 0

InfoUsr.Height = InfoUsr.Height + extraHeight
ActGrid.Height = ActGrid.Height + extraHeight
LbResum.Top = LbResum.Top + extraHeight
SesGrid.Top = SesGrid.Top + extraHeight
LbDates.Move BtnDates.Left, BtnDates.Top, BtnDates.Width, BtnDates.Height
LbDates.Caption = BtnDates.Caption
LbDates.Visible = True
BtnDates.Visible = False
InfoUsr.BackColor = &HFFFFFF
BtnAcaba.Visible = False
BtnPrint.Visible = False

On Error GoTo errprint
InfoUsr.PrintForm
On Error GoTo 0

InfoUsr.BackColor = &HC0C0C0
LbDates.Visible = False
BtnDates.Visible = True
InfoUsr.Height = formHeight
ActGrid.Height = ActGrid.Height - extraHeight
LbResum.Top = LbResum.Top - extraHeight
SesGrid.Top = SesGrid.Top - extraHeight
BtnAcaba.Visible = True
BtnPrint.Visible = True

InfoUsr.Refresh
'InfoUsr.Visible = True

Exit Sub

errprint:
MsgBox prnErr, 48
Resume Next


End Sub

Sub SesList_Click ()

screen.MousePointer = 11
FillActs SesList.ListIndex
screen.MousePointer = 0

End Sub

