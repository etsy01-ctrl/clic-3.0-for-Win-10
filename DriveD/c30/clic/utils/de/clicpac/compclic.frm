VERSION 2.00
Begin Form CompClic 
   BackColor       =   &H00C0C0C0&
   BorderStyle     =   3  'Fixed Double
   Caption         =   "ClicPac"
   ClientHeight    =   6120
   ClientLeft      =   1380
   ClientTop       =   1365
   ClientWidth     =   8550
   Height          =   6525
   Icon            =   COMPCLIC.FRX:0000
   Left            =   1320
   LinkTopic       =   "Form1"
   MaxButton       =   0   'False
   ScaleHeight     =   6120
   ScaleWidth      =   8550
   Top             =   1020
   Width           =   8670
   Begin CheckBox ChkNoChain 
      BackColor       =   &H00C0C0C0&
      Caption         =   "Untersucht keine Rueckmeldung von anderen Sammlungen"
      FontBold        =   0   'False
      FontItalic      =   0   'False
      FontName        =   "MS Sans Serif"
      FontSize        =   8.25
      FontStrikethru  =   0   'False
      FontUnderline   =   0   'False
      Height          =   255
      Left            =   120
      TabIndex        =   17
      Top             =   5280
      Width           =   4935
   End
   Begin CheckBox ChkInDir 
      BackColor       =   &H00C0C0C0&
      Caption         =   "Nicht außerhalb des Verzeichnisses der Sammlung suchen"
      FontBold        =   0   'False
      FontItalic      =   0   'False
      FontName        =   "MS Sans Serif"
      FontSize        =   8.25
      FontStrikethru  =   0   'False
      FontUnderline   =   0   'False
      Height          =   255
      Left            =   120
      TabIndex        =   16
      Top             =   5040
      Width           =   4875
   End
   Begin CommandButton BtnAjuda 
      BackColor       =   &H00C0C0C0&
      Caption         =   "Hilfe"
      Height          =   375
      Left            =   6840
      TabIndex        =   15
      Top             =   5700
      Width           =   1575
   End
   Begin Grid Gr 
      FixedCols       =   0
      FixedRows       =   0
      FontBold        =   0   'False
      FontItalic      =   0   'False
      FontName        =   "MS Sans Serif"
      FontSize        =   8.25
      FontStrikethru  =   0   'False
      FontUnderline   =   0   'False
      Height          =   3615
      HighLight       =   0   'False
      Left            =   5880
      Rows            =   1
      TabIndex        =   14
      Top             =   960
      Width           =   2415
   End
   Begin ListBox Tx 
      Height          =   3540
      Left            =   5880
      TabIndex        =   13
      Top             =   960
      Width           =   2415
   End
   Begin CommandButton BtnCopiar 
      BackColor       =   &H00C0C0C0&
      Caption         =   "kopieren..."
      Enabled         =   0   'False
      Height          =   375
      Left            =   4560
      TabIndex        =   12
      Top             =   5700
      Width           =   1575
   End
   Begin CommandButton BtnCompactar 
      BackColor       =   &H00C0C0C0&
      Caption         =   "komprimieren..."
      Enabled         =   0   'False
      Height          =   375
      Left            =   2760
      TabIndex        =   11
      Top             =   5700
      Width           =   1575
   End
   Begin CommandButton BtnExit 
      BackColor       =   &H00C0C0C0&
      Caption         =   "Schließen"
      Height          =   375
      Left            =   120
      TabIndex        =   10
      Top             =   5700
      Width           =   1695
   End
   Begin ListBox Lst 
      BackColor       =   &H00FFFFFF&
      Height          =   3930
      HelpContextID   =   400
      Left            =   3360
      TabIndex        =   6
      Top             =   840
      Width           =   2295
   End
   Begin Frame Frame1 
      BackColor       =   &H00C0C0C0&
      Caption         =   "FORM: "
      Height          =   975
      Left            =   120
      TabIndex        =   3
      Top             =   120
      Width           =   3135
      Begin OptionButton OpPcc 
         BackColor       =   &H00C0C0C0&
         Caption         =   "komprimierte Clic Sammlungen (*.PCC)"
         FontBold        =   0   'False
         FontItalic      =   0   'False
         FontName        =   "MS Sans Serif"
         FontSize        =   8.25
         FontStrikethru  =   0   'False
         FontUnderline   =   0   'False
         Height          =   255
         Left            =   60
         TabIndex        =   5
         Top             =   600
         Width           =   3015
      End
      Begin OptionButton OpPac 
         BackColor       =   &H00C0C0C0&
         Caption         =   "normale Clic Sammlungen (*.PAC)"
         FontBold        =   0   'False
         FontItalic      =   0   'False
         FontName        =   "MS Sans Serif"
         FontSize        =   8.25
         FontStrikethru  =   0   'False
         FontUnderline   =   0   'False
         Height          =   255
         Left            =   60
         TabIndex        =   4
         Top             =   240
         Value           =   -1  'True
         Width           =   2835
      End
   End
   Begin FileListBox File1 
      BackColor       =   &H00FFFFFF&
      Height          =   1590
      HelpContextID   =   400
      Left            =   120
      Pattern         =   "*.PAC"
      TabIndex        =   2
      Top             =   3240
      Width           =   3135
   End
   Begin DirListBox Dir1 
      BackColor       =   &H00FFFFFF&
      Height          =   1605
      Left            =   120
      TabIndex        =   1
      Top             =   1560
      Width           =   3135
   End
   Begin DriveListBox Drive1 
      BackColor       =   &H00FFFFFF&
      Height          =   315
      Left            =   120
      TabIndex        =   0
      Top             =   1200
      Width           =   3135
   End
   Begin Shape Shape1 
      Height          =   3855
      Left            =   5760
      Top             =   840
      Width           =   2655
   End
   Begin Image Pic 
      BorderStyle     =   1  'Fixed Single
      Height          =   3615
      Left            =   5880
      Stretch         =   -1  'True
      Top             =   960
      Width           =   2415
   End
   Begin Label LbNumFitxers 
      BackColor       =   &H00C0C0C0&
      Caption         =   "keine Dateien"
      Height          =   255
      Left            =   3360
      TabIndex        =   9
      Top             =   4800
      Width           =   4935
   End
   Begin Label LbCont 
      BackColor       =   &H00C0C0C0&
      Caption         =   "Bezug auf die Datei"
      Height          =   255
      Left            =   3360
      TabIndex        =   8
      Top             =   240
      Width           =   4935
   End
   Begin Label LbFitx 
      BackColor       =   &H00C0C0C0&
      Caption         =   "NOMFITXER"
      FontBold        =   0   'False
      FontItalic      =   0   'False
      FontName        =   "MS Sans Serif"
      FontSize        =   8.25
      FontStrikethru  =   0   'False
      FontUnderline   =   0   'False
      Height          =   255
      Left            =   3360
      TabIndex        =   7
      Top             =   480
      Width           =   4935
   End
End
Option Explicit

Sub BtnAjuda_Click ()

  SendKeys "{F1}"

End Sub

Private Sub BtnCompactar_Click ()

  If OpPac.Value = True Then
     cmpOn = True
     Load Compact
     Compact.Show
  Else
     dscOn = True
     Load Descomp
     Descomp.Show
  End If

End Sub

Sub BtnCopiar_Click ()
  
  If OpPac.Value = True Then
     copyOn = True
     Load Copia
     Copia.Show
  End If

End Sub

Private Sub BtnExit_Click ()

End

End Sub

Function checkCrides () As Integer

If Crides >= MaxItera Then
  If Not avisat Then
    MsgBox "Diese Sammlung hat eine komplexe Struktur. Das Programm wird mit der Suche fortsetzen, es kann aber sein, daß einzelne Dateien nicht gefunden werden.", 48, "ACHTUNG:"
    avisat = True
  End If
  checkCrides = True
Else
  checkCrides = False
End If

End Function

Private Sub Dir1_Change ()
  File1.Path = Dir1
End Sub

Private Sub Drive1_Change ()
  Dir1.Path = Drive1
End Sub

Private Sub File1_Click ()
  
  Dim a$, b$
  
  a$ = File1.Path
  b$ = File1.FileName
  
  If chkNoChain = False Then
    chPacs = True
  Else
    chPacs = False
  End If
  
  If b$ = "" Then
    Beep
    Exit Sub
  End If
  
  NetejaLlistaCompClic
  Crides = 0
  avisat = False
  CANCEL = False
  CDLG.LB1 = ""
  CDLG.LB2 = ""
  CDLG.Show
  popupOn = True
  DoEvents

  If OpPac.Value = True Then
    
    CDLG.LB1 = "Suche nach Bezug auf Dateien " + b$
    ScanPac a$, b$
    If Lst.ListCount > 0 Then
      BtnCompactar.Enabled = True
      BtnCopiar.Enabled = True
    End If
  
  Else
    
    CDLG.LB1 = "Den Inhalt der Datei analysieren " + b$
    ScanPcc a$, b$
    If Lst.ListCount > 0 Then BtnCompactar.Enabled = True
  
  End If

  Crides = 0
  popupOn = False
  CDLG.Hide
  Close
  If CANCEL Then NetejaLlistaCompClic

End Sub

Sub Form_Activate ()

  If popupOn Then
    CDLG.SetFocus
    Exit Sub
  End If
  If cmpOn Then
    Compact.SetFocus
    Exit Sub
  End If
  If dscOn Then
    Descomp.SetFocus
    Exit Sub
  End If
  If copyOn Then
    Copia.SetFocus
    Exit Sub
  End If

End Sub

Private Sub Form_Load ()

  Dim r As Integer

  Left = (screen.Width - Width) / 2
  Top = (screen.Height - Height) / 2
  app.HelpFile = app.Path + "\clicpac.hlp"
  FH = 1
  Crides = 0
  avisat = False
  CANCEL = False
  MaxItera = 48
  popupOn = False
  cmpOn = False
  dscOn = False
  copyOn = False
  chPacs = True
  pw = pic.Width
  ph = pic.Height
  ret$ = Chr$(&HD) + Chr$(&HA)
  ReDim LastRef$(0 To MaxItera)
  fnt$ = String$(255, 0)
  r = GetTempFileName(0, "cp", 0, fnt$)
  Kill fnt$
  GR.ColWidth(0) = 1000
  GR.ColWidth(1) = 10000
  NetejaLlistaCompClic

End Sub

Sub Form_Unload (CANCEL As Integer)

  Kill fnt$

End Sub

Function getRelativePath$ (fn$)

Dim i As Integer
getRelativePath$ = ""

For i = Len(fn$) To 1 Step -1
  If Mid$(fn$, i, 1) = "\" Then
    getRelativePath = Left$(fn$, i)
    Exit For
  End If
Next i
  
End Function

Function InList (fn$)

Dim i As Integer

InList = False
If CANCEL Then Exit Function

For i = 0 To Lst.ListCount
  If UCase$(Lst.List(i)) = UCase$(fn$) Then
    InList = True
    Exit Function
  End If
Next i

End Function

Sub Lst_Click ()

Dim p$, fn$, ext$
Dim r As Single, rh As Integer
Dim i As Integer, j As Integer, k As Integer
Dim t$, a$

t$ = Chr$(9)

pic.Picture = LoadPicture()
pic.Visible = False
Tx.Clear
Tx.Visible = False
GR.Visible = False
While GR.Rows > 1
  GR.RemoveItem 1
Wend

i = Lst.ListIndex
If i < 0 Then Exit Sub

fn$ = Trim(Lst.List(i))
If fn$ = "" Then Exit Sub
p$ = Dir1.Path
If Right$(p$, 1) <> "\" Then p$ = p$ + "\"
fn$ = p$ + fn$
  
ext$ = UCase$(Right$(fn$, 3))
Select Case ext$
  Case "BMP", "TXT", "ASS", "PUZ", "SOP", "TXA", "CRW", "GIF"
  Case Else
    Exit Sub
End Select

If OpPcc.Value = True Then
  fn$ = CompClic.LbFitx
  On Error GoTo DetectaErrorLst
  Open fn$ For Binary Access Read As #1 Len = 12000
  If CANCEL Then
    Close
    CANCEL = False
    Exit Sub
  End If
  On Error GoTo 0
  fn$ = fnt$
  DesCompFile i + 1, fn$
  Close
End If

Select Case ext$
  Case "BMP"
    'p$ = Dir1.Path
    pic.Visible = False
    pic.stretch = False
    On Error GoTo DetectaErrorLst
    pic.Picture = LoadPicture(fn$)
    If CANCEL Then Exit Sub
    If pic.Width > pw Or pic.Height > ph Then
      If pic.Width / pic.Height > pw / ph Then
        r = pw / pic.Width
      Else
        r = ph / pic.Height
      End If
      pic.Width = pic.Width * r
      pic.Height = pic.Height * r
      pic.stretch = True
    End If
    pic.Visible = True
  
  Case "GIF"
    Tx.AddItem "Datei GIF"
    Tx.Visible = True
  
  Case "TXT"
    On Error GoTo DetectaErrorLst
    Open fn$ For Input As #1
    If CANCEL Then Exit Sub
    Do While Not EOF(1)
      Line Input #1, p$
      If CANCEL Then Exit Sub
      Tx.AddItem p$
      'Tx.Text = Tx.Text + p$ + Chr$(13) + Chr$(10)
    Loop
    Close #1
    Tx.Visible = True
  
  Case "PUZ", "ASS", "SOP", "CRW", "TXA"
    On Error GoTo DetectaErrorLst
    Open fn$ For Input As #1
    If CANCEL Then Exit Sub
    LoadGAct 1, ext$, fn$
    Close #1
    If CANCEL Then Exit Sub
    GR.Row = 0
    GR.Col = 0: GR.Text = "Version:"
    GR.Col = 1: GR.Text = GAct.Ver
    rh = GR.RowHeight(GR.Row) - 30
    GR.AddItem "Beschreibung: " + t$ + GAct.Desc
    For i = 1 To 3
      a$ = GAct.Font(i)
      If a$ <> "" Then
        For j = Len(a$) To 1 Step -1
          If Mid$(a$, j, 1) = "," Then Exit For
        Next j
        GR.AddItem "Hintergrund" + Str$(i) + ": " + t$ + Left$(a$, j - 1) + ret$ + Right$(a$, Len(a$) - j)
        GR.RowHeight(GR.Rows - 1) = rh * 2 + 30
      End If
      If GAct.Ver < 118 Then Exit For
      GR.AddItem "Farbe" + Str$(i) + ":" + t$ + GAct.Colors(i)
    Next i
    If GAct.Ver > 117 Then
      GR.AddItem "Optionen: " + t$ + GAct.Opcions
    End If
    If GAct.Ver > 118 Then
      GR.AddItem "Fons: " + t$ + GAct.bmpFons
    End If
    GR.AddItem "MInicial: " + t$ + GAct.InitMess
    GR.AddItem "MFinal: " + t$ + GAct.EndMess
    For i = 1 To 3
      If GAct.FileNameCont(i) <> "" Then
        GR.AddItem "Cont" + Trim$(Str$(i)) + ": " + t$ + GAct.FileNameCont(i)
      End If
    Next i
    GR.AddItem "XCas1: " + t$ + Trim$(Str$(GAct.TxtCW))
    If GAct.TxtCW2 > 0 Then GR.AddItem "XCas2: " + t$ + Trim$(Str$(GAct.TxtCW2))
    GR.AddItem "YCas1: " + t$ + Trim$(Str$(GAct.TxtCH))
    If GAct.TxtCH2 > 0 Then GR.AddItem "YCas2: " + t$ + Trim$(Str$(GAct.TxtCH2))
    GR.AddItem "Delim0: " + t$ + SiNo(GAct.Delim0)
    GR.AddItem "Delim1: " + t$ + SiNo(GAct.Delim1)
    GR.AddItem "BTAjuda: " + t$ + SiNo(GAct.shHelp)
    GR.AddItem "Pos: " + t$ + Trim$(Str$(GAct.graPos))
    GR.AddItem "BTPuz: " + t$ + SiNo(GAct.shPuz)
    GR.AddItem "NoReinit: " + t$ + SiNo(GAct.noBV)
    GR.AddItem "InvAss: " + t$ + SiNo(GAct.invAss)
    GR.AddItem "TileImg: " + t$ + SiNo(GAct.tileBmp)
    GR.AddItem "Transparent: " + t$ + SiNo(GAct.pwTransp)
    GR.AddItem "Posicionat: " + t$ + SiNo(GAct.pwrp)
    
    If GAct.shPuz <> 0 Then
      GR.AddItem "CustHlp: " + t$ + SiNo(GAct.custHlp)
      GR.AddItem "HlpTopic: " + t$ + GAct.hlpTopic
      GR.AddItem "HlpFile: " + t$ + GAct.custHelpFile
    End If
    If GAct.useDLL <> 0 Then
      GR.AddItem "UseDLL: " + t$ + SiNo(GAct.useDLL)
      GR.AddItem "DLL: " + t$ + GAct.rgDLL
      GR.AddItem "DLLOptions: " + t$ + GAct.DLLOptions
    End If
    GR.AddItem "ActMode: " + t$ + Str$(GAct.actMode)
    GR.AddItem "NumCasW: " + t$ + Str$(GAct.ncw)
    GR.AddItem "NumCasH: " + t$ + Str$(GAct.nch)
    GR.AddItem "PuzMode: " + t$ + Str$(GAct.puzMode)
    GR.AddItem "Comprim: " + t$ + SiNo(GAct.stretch)
    GR.AddItem "Barreja1: " + t$ + SiNo(GAct.bar(1))
    GR.AddItem "Barreja2: " + t$ + SiNo(GAct.bar(2))
    GR.AddItem "Solució: " + t$ + SiNo(GAct.sol)
    GR.AddItem "MultipleAss: " + t$ + SiNo(GAct.mAss)
    If GAct.nctxw > 0 Or GAct.nctxh > 0 Then
      GR.AddItem "NCasTxX: " + t$ + Trim$(Str$(GAct.nctxw))
      GR.AddItem "NCasTxY: " + t$ + Trim$(Str$(GAct.nctxh))
      If GAct.nctxh > 0 And GAct.graTxt(1) <> "" Then
        k = 0
        a$ = "Graella:" + t$
        For i = 1 To GAct.nctxh
          a$ = a$ + GAct.graTxt(i) + ret$
          k = k + 1
          If i = MAXL Then Exit For
        Next i
        GR.AddItem a$
        GR.RowHeight(GR.Rows - 1) = rh * k + 30
      End If
    End If
    For j = 1 To 2
      If GAct.ntags(j) > 0 Then
        GR.AddItem "NumTags" + Trim$(Str$(j)) + ": " + t$ + Trim$(Str$(GAct.ntags(j)))
        k = 0
        a$ = "Tags" + Str$(j) + ":" + t$
        For i = 1 To GAct.ntags(j)
          a$ = a$ + GAct.tags(j, i) + ret$
          k = k + 1
          If i = MAXL Then Exit For
        Next i
        GR.AddItem a$
        GR.RowHeight(GR.Rows - 1) = rh * k + 30
      End If
    Next j
    If (GAct.colorUsrErr <> "") Then
      GR.AddItem "ColorUsrErr: " + t$ + GAct.colorUsrErr
      GR.AddItem "Opcions2: " + t$ + GAct.Opcions2
      GR.AddItem "TxtPrev: " + t$ + GAct.txPrevi
      GR.AddItem "Text: " + t$ + GAct.txEx
    End If
    GR.Visible = True
  Case Else
End Select

If OpPcc.Value = True Then Kill fnt$

Exit Sub
DetectaErrorLst:
MissatgeError fn$
Resume Next

End Sub

Private Sub OpPac_Click ()

  If File1.Pattern <> "*.PAC" Then
    NetejaLlistaCompClic
    File1.Pattern = "*.PAC"
    BtnCompactar.Caption = "komprimieren..."
  End If

End Sub

Private Sub OpPcc_Click ()

  If File1.Pattern <> "*.PCC" Then
    NetejaLlistaCompClic
    File1.Pattern = "*.PCC"
    BtnCompactar.Caption = "dekomprimieren..."
  End If

End Sub

Sub ScanFile (fp$, rp$, fn$)

Dim ext As String
Dim b$, c$

DoEvents
If CANCEL Then Exit Sub
If checkCrides() Then Exit Sub

If fn$ = "" Then Exit Sub
If InStr(fn$, ".") = 0 Then Exit Sub

If ChkInDir.Value <> 0 Then
  If InStr(fn$, "\") <> 0 Then Exit Sub
End If

rp$ = simplifyPath(rp$)

fn$ = Trim(fn$)
If Len(fn$) < 4 Then Exit Sub

'Afegit
b$ = simplifyPath(rp$ + fn$)
'fins aqui

If InList(b$) Then Exit Sub
'If InList(simplifyPath(rp$ + fn$)) Then Exit Sub

ext = UCase$(Right$(fn$, 4))
If ext = ".PAC" And chPacs = False And nfx > 0 Then Exit Sub

Crides = Crides + 1
Lst.AddItem b$
'Lst.AddItem rp$ + fn$
CDLG.LB2 = fn$
DoEvents
nfx = nfx + 1

Select Case ext
  Case ".PAC"
    npqx = npqx + 1
    UpdCaption
    ScanFilePac fp$, rp$ + fn$
  Case ".TXT"
    UpdCaption
    ScanFileTxt fp$, rp$, fn$
  Case ".ASS", ".SOP", ".PUZ", ".CRW", ".TXA"
    nactx = nactx + 1
    UpdCaption
    ScanFileAct fp$, rp$ + fn$
  Case Else
End Select

Crides = Crides - 1

End Sub

Sub ScanFileAct (fp$, fn$)

Dim l$, a$, rp$
Dim Ver As Integer, i As Integer, h As Integer, nl As Integer, nc As Integer

If CANCEL Then Exit Sub

h = FH: FH = FH + 1
rp$ = getRelativePath(fn$)

On Error GoTo DetectaError
Open fp$ + "\" + fn$ For Input As h
If CANCEL Then Exit Sub
On Error GoTo 0
LastRef$(Crides) = fn$

  Line Input #h, l$
  Ver = Val(l$): If Ver > 1000 Then Ver = Ver - 1000
  If Ver >= 100 Then
    If Ver > 107 Then
      'Descripció de l'activitat
      Do While Not EOF(h)
        Line Input #h, l$
        If l$ <> "" Then
          If Asc(l$) = &H87 Then Exit Do
        End If
      Loop
    End If
    
    'Tipus de lletra
    If Ver > 100 Then
      Line Input #h, l$
      If Ver > 117 Then
        For i = 1 To 5
          Line Input #h, l$
        Next i
      End If
    End If

    'Botons i comptadors
    If Ver > 117 Then Line Input #h, l$
        
    'Imatge de fons
    If Ver > 118 Then
      Line Input #h, l$
      ScanFile fp$, rp$, l$
    End If
    
    'Missatges inicial i final
    Line Input #h, l$: ScanTxLin fp$, rp$, l$
    Line Input #h, l$: ScanTxLin fp$, rp$, l$
    
    If Ver > 102 Then Line Input #h, l$
    If Ver > 105 Then
      Line Input #h, l$
      Line Input #h, l$
    End If
    
    'Fitxer d'ajuda
    If Ver > 108 Then
      Line Input #h, l$
      Line Input #h, l$
      Line Input #h, l$: ScanFile fp$, rp$, l$
    End If
    
    'Fitxer DLL
    If Ver > 114 Then
      Line Input #h, l$
      Line Input #h, l$: ScanFile fp$, rp$, l$
      Line Input #h, l$
    End If

    Select Case UCase$(Right$(fn$, 4))
      Case ".PUZ"
        Line Input #h, l$: ScanFile fp$, rp$, l$
        ' venen més coses... però no són fitxers
        
      Case ".ASS"
        If Ver > 109 Then Line Input #h, l$
        Line Input #h, l$
        Line Input #h, l$
        Line Input #h, l$: ScanFile fp$, rp$, l$
        Line Input #h, l$: ScanFile fp$, rp$, l$
        Line Input #h, l$
        Line Input #h, l$
        If Ver > 106 Then
          Line Input #h, l$
          If Val(l$) <> 0 Then
            Line Input #h, l$: ScanFile fp$, rp$, l$
          End If
        End If
        ' venen més coses... però no són fitxers
        
      Case ".SOP"
        Line Input #h, l$:
        If l$ <> "" Then
          If Left$(l$, 1) <> "*" Then ScanFile fp$, rp$, l$
        End If
         ' venen més coses... però no són fitxers.
    
      Case ".CRW"
        Line Input #h, l$: nc = Val(l$)
        Line Input #h, l$: nl = Val(l$)
        For i = 1 To nl: Line Input #h, l$: Next i
        For i = 1 To nc + nl
          Line Input #h, l$: ScanTxLin fp$, rp$, l$
        Next i
            
      Case ".TXA"
        For i = 1 To 5: Line Input #h, l$: Next i
        For i = 1 To 2
          l$ = ""
          Do While Not EOF(h)
            Line Input #h, a$
            If a$ <> "" Then
              If Asc(a$) = &H87 Then Exit Do
              l$ = l$ & a$
            End If
          Loop
          If l$ <> "" Then ScanTxLin fp$, rp$, l$
        Next i
    
    End Select
  End If

Close h
FH = FH - 1

Exit Sub
DetectaError:
MissatgeError fn$
Resume Next

End Sub

Sub ScanFilePac (fp$, fn$)

Dim l$, rp$
Dim Ver As Integer, i As Integer, h As Integer

If CANCEL Then Exit Sub

h = FH: FH = FH + 1
rp$ = getRelativePath(fn$)
On Error GoTo DetectaErrorSFPC
Open fp$ + "\" + fn$ For Input As h
On Error GoTo 0
If CANCEL Then Exit Sub
LastRef$(Crides) = fn$

  Line Input #h, l$
  Ver = Val(l$): If Ver > 1000 Then Ver = Ver - 1000
  If Ver > 107 Then
    'Descripció del paquet
    Do While Not EOF(h)
      Line Input #h, l$
      If l$ <> "" Then
        If Asc(l$) = &H87 Then Exit Do
      End If
    Loop
    
    'Opcions d'encadenament i pas automàtic
    If Ver >= 113 Then
      For i = 1 To 3: Line Input #h, l$: Next i
      For i = 1 To 3
        'Fitxers d'encadenament
        Line Input #h, l$
        If chPacs Then ScanFile fp$, rp$, l$
      Next i
      For i = 1 To 4: Line Input #h, l$: Next i
    End If
  End If
  'Fitxers del paquet
  Do While Not EOF(h)
    Line Input #h, l$
    ScanFile fp$, rp$, l$
  Loop

Close h
FH = FH - 1

Exit Sub
DetectaErrorSFPC:
MissatgeError fn$
Resume Next

End Sub

Sub ScanFileTxt (fp$, rp$, fn$)

Dim i As Integer, k As Integer, h As Integer
Dim l$, ch$

If CANCEL Then Exit Sub

If checkCrides() Then Exit Sub

h = FH: FH = FH + 1
On Error GoTo DetectaErrorSFT
Open fp$ + "\" + rp$ + fn$ For Input As h
If CANCEL Then Exit Sub
On Error GoTo 0
LastRef$(Crides) = fn$

  Do While Not EOF(h)
    Line Input #h, l$
    ScanTxLin fp$, rp$, l$
  Loop
  
Close h
FH = FH - 1

Exit Sub
DetectaErrorSFT:
MissatgeError fn$
Resume Next

End Sub

Sub ScanPac (fp$, fn$)

  NetejaLlistaCompClic
  LbCont.Caption = "Bezug auf die Datei"
  LbFitx.Caption = fp$ + "\" + fn$

  ScanFile fp$, "", fn$
  
End Sub

Sub ScanPcc (fp$, fn$)
  
  Dim pcc As PccEntry
  Dim v$
  Dim i As Integer
  
  LbCont.Caption = "komprimierte Dateien in:"
  LbFitx.Caption = fp$ + "\" + fn$

  'Screen.MousePointer = 11
  
  On Error GoTo DetectaErrorSCPCC
  Open LbFitx.Caption For Binary Access Read As #1
  If CANCEL Then Exit Sub
  
  For i = 1 To 5
    v$ = v$ + Input(1, 1)
    If CANCEL Then Exit Sub
  Next i
    
  If v$ <> "PCC00" Then
    MsgBox "Fehler: Diese Datei kann nicht mit ClicFile 2.1 erzeugt werden"
    Exit Sub
  End If
  
  Do While Not EOF(1)
    If Input(1, 1) = Chr$(&H87) Then Exit Do
    If CANCEL Then Exit Sub
  Loop
  
  For i = 1 To 2
    v$ = Input(1, 1)
    If CANCEL Then Exit Sub
  Next i
    
  Get #1, , nfpcc
  If CANCEL Then Exit Sub
  Get #1, , offz
  If CANCEL Then Exit Sub
  
  ReDim PccDir(nfpcc) As PccEntry
  
  For i = 1 To nfpcc
    GetEntry 1, pcc
    If CANCEL Then Exit Sub
    PccDir(i) = pcc
    Lst.AddItem pcc.name
    nfx = nfx + 1
    Select Case UCase$(Right$(pcc.name, 4))
      Case ".PAC"
        npqx = npqx + 1
      Case ".ASS", ".PUZ", ".SOP", ".CRW", ".TXA"
        nactx = nactx + 1
    End Select
    UpdCaption
    DoEvents
  Next i
  
  Close #1
  
Exit Sub
DetectaErrorSCPCC:
MissatgeError fn$
Resume Next

End Sub

Sub ScanTxLin (fp$, rp$, l$)

Dim i As Integer, k As Integer, ch$

If CANCEL Then Exit Sub

If checkCrides() Then Exit Sub

If l$ <> "" Then
  For i = 1 To Len(l$)
    ch$ = Mid$(l$, i, 1)
    If ch$ = "{" Then
      For k = i + 1 To Len(l$)
        ch$ = Mid$(l$, k, 1)
        If ch$ = "}" Or ch$ = " " Then
          ScanFile fp$, rp$, Mid$(l$, i + 1, k - i - 1)
          Exit For
        End If
      Next k
      i = k
    End If
  Next i
End If

End Sub

Sub UpdCaption ()

  Dim a$

  a$ = nfx & " Dateien"
  If npqx > 0 Then
    a$ = a$ & " - " & npqx
    If npqx < 2 Then
      a$ = a$ & " Sammlung"
    Else
      a$ = a$ & " Sammlungen"
    End If
  End If
  If nactx > 0 Then
    a$ = a$ & " - " & nactx
    If nactx < 2 Then
      a$ = a$ & " Übung"
    Else
      a$ = a$ & " Übungen"
    End If
  End If
  
  CompClic.LbNumFitxers = a$

End Sub

