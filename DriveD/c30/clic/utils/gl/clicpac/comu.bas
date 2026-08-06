Option Explicit

Global FH As Integer
Global Crides As Integer
Global avisat As Integer
Global CANCEL As Integer
Global MaxItera As Integer
Global nfpcc As Integer
Global offz As Long
Global popupOn As Integer
Global cmpOn As Integer
Global copyOn As Integer
Global dscOn As Integer
Global ret$
Global LastRef$()
Global pw As Integer, ph As Integer
Global fnt$
Global nfx As Integer, nactx As Integer, npqx As Integer
Global chPacs As Integer

Type PccEntry
  name As String
  offset As Long
  length As Long
End Type

Global PccDir() As PccEntry

Declare Function GetTempFileName Lib "kernel" (ByVal drive%, ByVal pref$, ByVal prefix%, ByVal tempFName$) As Integer

Global Const MAXL = 20
Type Act
  Ver As Integer
  Desc As String
  Font(4) As String
  Colors(4) As String
  Opcions As String
  InitMess As String
  EndMess As String
  Delim0 As Integer
  Delim1 As Integer
  TxtCW As Integer
  TxtCW2 As Integer
  TxtCH As Integer
  TxtCH2 As Integer
  shHelp As Integer
  graPos As Integer
  shPuz As Integer
  noBV As Integer
  custHlp As Integer
  hlpTopic As String
  custHelpFile As String
  useDLL As Integer
  rgDLL As String
  DLLOptions As String
  actMode As Integer
  FileNameCont(3) As String
  ncw As Integer
  nch As Integer
  puzMode As Integer
  stretch As Integer
  bar(2) As Integer
  sol As Integer
  mAss As Integer
  nctxw As Integer
  nctxh As Integer
  ass(MAXL) As String
  ntags(2) As Integer
  tags(2, MAXL) As String
  graTxt(MAXL) As String
  bmpFons As String
  invAss As Integer
  tileBmp As Integer
  pwTransp As Integer
  pwrp As Integer
  colorUsrErr As String
  Opcions2 As String
  txBtCorregir As String
  txPrevi As String
  txEx As String
End Type

Global GAct As Act

Sub ClearGAct ()
  
  Dim i As Integer, j As Integer

  GAct.Ver = 0
  GAct.Desc = ""
  For i = 1 To 4
    GAct.Font(i) = ""
    GAct.Colors(i) = ""
  Next i
  GAct.Opcions = ""
  GAct.InitMess = ""
  GAct.EndMess = ""
  GAct.Delim0 = False
  GAct.Delim1 = False
  GAct.TxtCW = 0
  GAct.TxtCW2 = 0
  GAct.TxtCH = 0
  GAct.TxtCH2 = 0
  GAct.shHelp = False
  GAct.graPos = 0
  GAct.shPuz = False
  GAct.noBV = False
  GAct.custHlp = False
  GAct.hlpTopic = ""
  GAct.custHelpFile = ""
  GAct.useDLL = False
  GAct.rgDLL = ""
  GAct.DLLOptions = ""
  GAct.actMode = 0
  GAct.FileNameCont(1) = ""
  GAct.FileNameCont(2) = ""
  GAct.FileNameCont(3) = ""
  GAct.ncw = 0
  GAct.nch = 0
  GAct.puzMode = 0
  GAct.stretch = False
  GAct.bar(1) = False
  GAct.bar(2) = False
  GAct.sol = False
  GAct.mAss = False
  GAct.nctxw = 0
  GAct.nctxh = 0
  For i = 1 To MAXL
    GAct.ass(i) = ""
  Next i
  For i = 1 To 2
    GAct.ntags(i) = 0
  Next i
  For j = 1 To 2
    For i = 1 To MAXL
      GAct.tags(2, MAXL) = ""
    Next i
  Next j
  For i = 1 To MAXL
    GAct.graTxt(i) = ""
  Next i
  GAct.bmpFons = ""
  GAct.invAss = False
  GAct.tileBmp = False
  GAct.pwTransp = False
  GAct.pwrp = False
  GAct.colorUsrErr = ""
  GAct.Opcions2 = ""
  GAct.txBtCorregir = ""
  GAct.txPrevi = ""
  GAct.txEx = ""

End Sub

Sub DesCompFile (i As Integer, fn$)
      
Dim l As Long, l2 As Long
Dim s As String
  
On Error GoTo DetectaErrorDescompFile
Open fn$ For Binary Access Write As #2 Len = 12000
If CANCEL Then Exit Sub

Seek #1, PccDir(i).offset + 1
l = PccDir(i).length
Do While l > 0 And CANCEL = False
  If l > 12000 Then l2 = 12000 Else l2 = l
  s = Input(l2, #1)
  Put #2, , s
  l = l - l2
  DoEvents
Loop
Close #2

Exit Sub
DetectaErrorDescompFile:
MissatgeError fn$
Resume Next

End Sub

Sub GetEntry (fn As Integer, pcc As PccEntry)

Dim b As String * 1
Dim l As Long

pcc.name = ""
pcc.offset = 0
pcc.length = 0

If EOF(fn) Then Exit Sub
Do While Not EOF(fn)
  Get #fn, , b
  If b = Chr$(0) Then Exit Do
  pcc.name = pcc.name + b
Loop
Get #fn, , pcc.offset
Get #fn, , pcc.length

End Sub

Sub LoadGAct (h As Integer, ext$, fn$)

  Dim l$
  Dim Ver As Integer, bp As Integer
  Dim i As Integer, j As Integer

  ClearGAct
  bp = 0
  On Error GoTo DetectaErrorLGA

  Line Input #h, l$
  Ver = Val(l$): If Ver > 1000 Then Ver = Ver - 1000
  GAct.Ver = Ver
  If Ver >= 100 Then
    If Ver > 107 Then
      'Descripció de l'activitat
      Do While Not EOF(h)
        Line Input #h, l$
        If l$ <> "" Then
          If Asc(l$) = &H87 Then
            Exit Do
          Else
            GAct.Desc = GAct.Desc + l$
          End If
        End If
      Loop
    End If
    
    'Tipus de lletra
    If Ver > 100 Then
      If Ver > 117 Then
        For i = 1 To 3
          Line Input #h, GAct.Font(i)
          Line Input #h, GAct.Colors(i)
        Next i
      Else
        Line Input #h, GAct.Font(1)
      End If
    End If
    
    If Ver > 117 Then Line Input #h, GAct.Opcions

    If Ver > 118 Then Line Input #h, GAct.bmpFons

    'Missatges inicial i final
    Line Input #h, GAct.InitMess
    Line Input #h, GAct.EndMess
    
    If Ver > 102 Then
      Line Input #h, l$: bp = Val(l$)
      GAct.Delim0 = bp And 1
      GAct.Delim1 = bp And 2
    End If
    If Ver > 105 Then
      Line Input #h, l$
      GAct.TxtCW = Val(l$)
      i = InStr(l$, ";")
      If i <> 0 Then GAct.TxtCW2 = Val(Right$(l$, Len(l$) - i))
      Line Input #h, l$
      GAct.TxtCH = Val(l$)
      i = InStr(l$, ";")
      If i <> 0 Then GAct.TxtCH2 = Val(Right$(l$, Len(l$) - i))
    End If
    GAct.shHelp = bp And 4
    GAct.graPos = (bp And (8 + 16)) / 8
    GAct.shPuz = bp And 32
    GAct.noBV = bp And 64
    GAct.invAss = bp And 128
    GAct.tileBmp = bp And 256
    GAct.pwTransp = bp And 512
    GAct.pwrp = bp And 1024

    'Fitxer d'ajuda
    If Ver > 108 Then
      Line Input #h, l$: GAct.custHlp = Val(l$)
      Line Input #h, GAct.hlpTopic
      Line Input #h, GAct.custHelpFile
    End If
    
    'Fitxer DLL
    If Ver > 114 Then
      Line Input #h, l$: GAct.useDLL = Val(l$)
      Line Input #h, GAct.rgDLL
      Line Input #h, GAct.DLLOptions
    End If

    Select Case ext$
      Case "PUZ"
        Line Input #h, GAct.FileNameCont(1)
        Line Input #h, l$: GAct.ncw = Val(l$)
        Line Input #h, l$: GAct.nch = Val(l$)
        Line Input #h, l$: GAct.puzMode = Val(l$)
        Line Input #h, l$: GAct.stretch = Val(l$)

        
      Case "ASS"
        If Ver > 109 Then
          Line Input #h, l$: GAct.puzMode = Val(l$)
        End If
        Line Input #h, l$: GAct.ncw = Val(l$)
        Line Input #h, l$: GAct.nch = Val(l$)
        Line Input #h, GAct.FileNameCont(1)
        Line Input #h, GAct.FileNameCont(2)
        Line Input #h, l$: GAct.bar(1) = Val(l$)
        Line Input #h, l$: GAct.bar(2) = Val(l$)
        If Ver > 106 Then
          Line Input #h, l$: GAct.sol = Val(l$)
          If GAct.sol <> 0 Then
            Line Input #h, GAct.FileNameCont(3)
          End If
        End If
        If Ver > 101 Then
          Line Input #h, l$: GAct.mAss = Val(l$)
          If GAct.mAss <> 0 Then
             Line Input #h, l$: GAct.nctxw = Val(l$)
             Line Input #h, l$: GAct.nctxh = Val(l$)
             j = GAct.ncw * GAct.nch
             For i = 1 To MAXL
               If i > j Then Exit For
               Line Input #h, l$: GAct.ass(i) = l$
             Next i
          End If
          
        End If
        
      Case "SOP"
        Line Input #h, l$
        If l$ <> "" Then
          If Left$(l$, 1) <> "*" Then
            GAct.bar(1) = True
            GAct.FileNameCont(1) = l$
          End If
        End If
        Line Input #h, l$: GAct.ncw = Val(l$)
        Line Input #h, l$: GAct.nch = Val(l$)
        Line Input #h, l$: GAct.nctxw = Val(l$)
        Line Input #h, l$: GAct.nctxh = Val(l$)
        Line Input #h, l$: GAct.ntags(1) = Val(l$)
        j = GAct.nctxh
        For i = 1 To j
          Line Input #h, l$
          If i <= MAXL Then GAct.graTxt(i) = l$
        Next i
        j = GAct.ntags(1)
        For i = 1 To j
          Line Input #h, l$
          If i <= MAXL Then GAct.tags(1, i) = l$
        Next i

    
      Case "CRW"
        Line Input #h, l$: GAct.nctxw = Val(l$)
        GAct.ntags(2) = GAct.nctxw
        Line Input #h, l$: GAct.nctxh = Val(l$)
        GAct.ntags(1) = GAct.nctxh
        j = GAct.nctxh
        For i = 1 To j
          Line Input #h, l$
          If i <= MAXL Then GAct.graTxt(i) = l$
        Next i
        j = GAct.ntags(1)
        For i = 1 To j
          Line Input #h, l$
          If i <= MAXL Then GAct.tags(1, i) = l$
        Next i
        j = GAct.ntags(2)
        For i = 1 To j
          Line Input #h, l$
          If i <= MAXL Then GAct.tags(2, i) = l$
        Next i

      Case "TXA"
          Line Input #h, GAct.Font(4)
          Line Input #h, GAct.Colors(4)
          Line Input #h, GAct.colorUsrErr
          Line Input #h, GAct.Opcions2
          Line Input #h, GAct.txBtCorregir

          Do While Not EOF(h)
            Line Input #h, l$
            If l$ <> "" Then
              If Asc(l$) = &H87 Then
                Exit Do
              Else
                GAct.txEx = GAct.txEx + l$
              End If
            End If
          Loop
          If GAct.txEx <> "" Then
            If Len(GAct.txEx) > 30 Then GAct.txEx = Left(GAct.txEx, 30) & "..."
          End If

          Do While Not EOF(h)
            Line Input #h, l$
            If l$ <> "" Then
              If Asc(l$) = &H87 Then
                Exit Do
              Else
                GAct.txPrevi = GAct.txPrevi + l$
              End If
            End If
          Loop
          If GAct.txPrevi <> "" Then
            If Len(GAct.txPrevi) > 30 Then GAct.txPrevi = Left(GAct.txPrevi, 30) & "..."
          End If
          

    End Select
  End If



Exit Sub
DetectaErrorLGA:
MissatgeError fn$
Resume Next

End Sub

Sub MissatgeError (fn$)

Dim a$

a$ = "Erro accedendo ó arquivo: " + fn$ + ": "
a$ = a$ + ret$ + Error(Err)
If Crides > 0 Then
  If LastRef$(Crides - 1) <> "" Then a$ = a$ + ret$ + "(referenciado por: " + LastRef$(Crides - 1) + ")"
End If
MsgBox a$, 16, "ERRO"
CANCEL = True

End Sub

Sub NetejaLlistaCompClic ()

  nfx = 0
  nactx = 0
  npqx = 0
  CompClic.Lst.Clear
  CompClic.LbFitx.Caption = ""
  CompClic.LbCont.Caption = ""
  CompClic.LbNumFitxers = "0 arquivos"
  CompClic.BtnCompactar.Enabled = False
  CompClic.BtnCopiar.Enabled = False
  CompClic.Pic.Visible = False
  CompClic.Tx.Visible = False
  CompClic.Tx.Clear
  CompClic.Gr.Visible = False
  While CompClic.Gr.Rows > 1
    CompClic.Gr.RemoveItem 1
  Wend
  CompClic.File1.Refresh
  Crides = 0
  DoEvents
  
End Sub

Function OverTest (fn$) As Integer

Dim i As Integer
Dim pt$, a$

OverTest = True

For i = Len(fn$) To 1 Step -1
  If Mid$(fn$, i, 1) = "\" Then Exit For
Next i
pt$ = Left$(fn$, i - 1)

If Dir(pt$, 16) = "" Then
  i = MsgBox("O directorio: " + pt$ + " non existe. ¿Queredes crealo?", 3 + 32)
  Select Case i
    Case 6 'yes
      MkDir pt$
    Case 2 'cancel
      OverTest = False
      CANCEL = True
      Exit Function
   Case Else
      OverTest = False
      Exit Function
   End Select
End If

If Dir(fn$) <> "" Then
  i = MsgBox("O arquivo: " + fn$ + " xa existe. ¿Queredes substituilo?", 3 + 32)
  Select Case i
    Case 6 'yes
      OverTest = True
    Case 2 'cancel
      OverTest = False
      CANCEL = True
    Case Else
      OverTest = False
  End Select
End If
  
End Function

Function simplifyPath (p$) As String

  Dim a$, b$
  Dim i As Integer, j As Integer

  simplifyPath = ""
  If (p$ = "") Then Exit Function

  a$ = p$
  i = InStr(p$, "\..\")
  Do While (i > 1)
    For j = i - 1 To 1 Step -1
      If Mid$(a$, j, 1) = "\" Then Exit For
    Next j
    If j > 0 Then b$ = Left$(a$, j) Else b$ = ""
    j = i + 4
    If j <= Len(a$) Then b$ = b$ + Right$(a$, Len(a$) - j + 1)
    a$ = b$
    'If a$ <> "" Then i = InStr(p$, "\..\") Else i = 0
    If a$ <> "" Then i = InStr(a$, "\..\") Else i = 0
  Loop

  simplifyPath = a$
  
End Function

Function SiNo (i As Integer) As String

  If i = 0 Then SiNo = "non" Else SiNo = "si"

End Function

