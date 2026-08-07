VERSION 5.00
Begin VB.Form TestClic 
   Caption         =   "Form1"
   ClientHeight    =   6405
   ClientLeft      =   60
   ClientTop       =   345
   ClientWidth     =   10260
   LinkTopic       =   "Form1"
   ScaleHeight     =   6405
   ScaleWidth      =   10260
   StartUpPosition =   3  'Windows Default
   Begin VB.ListBox lsterr 
      Height          =   5130
      Left            =   5280
      TabIndex        =   7
      Top             =   1200
      Width           =   4695
   End
   Begin VB.ListBox Lst 
      Height          =   5715
      Left            =   3000
      TabIndex        =   5
      Top             =   600
      Width           =   2175
   End
   Begin VB.FileListBox File1 
      Height          =   2820
      Left            =   120
      Pattern         =   "*.PCC"
      TabIndex        =   2
      Top             =   3480
      Width           =   2655
   End
   Begin VB.DirListBox Dir1 
      Height          =   2790
      Left            =   120
      TabIndex        =   1
      Top             =   600
      Width           =   2655
   End
   Begin VB.DriveListBox Drive1 
      Height          =   315
      Left            =   120
      TabIndex        =   0
      Top             =   240
      Width           =   2655
   End
   Begin VB.Label lbMsgScan 
      Caption         =   "------"
      Height          =   255
      Left            =   5280
      TabIndex        =   6
      Top             =   720
      Width           =   4695
   End
   Begin VB.Label LbFitx 
      Caption         =   "--------"
      Height          =   255
      Left            =   5640
      TabIndex        =   4
      Top             =   120
      Width           =   4095
   End
   Begin VB.Label LbCont 
      Caption         =   "Anàlisi de paquets Clic"
      Height          =   255
      Left            =   3120
      TabIndex        =   3
      Top             =   120
      Width           =   2415
   End
End
Attribute VB_Name = "TestClic"
Attribute VB_GlobalNameSpace = False
Attribute VB_Creatable = False
Attribute VB_PredeclaredId = True
Attribute VB_Exposed = False
Option Explicit

Private Sub Dir1_Change()

  File1.Path = Dir1

End Sub

Private Sub Dir1_Click()

  File1.Path = Dir1.List(Dir1.ListIndex)

End Sub

Private Sub Drive1_Change()
  
  Dir1.Path = Drive1
  
End Sub

Private Sub File1_Click()

  Dim i As Integer
  Dim f As String
  Dim ext As String
  
  NetejaLlistaCompClic
  lsterr.Clear
  Crides = 1
  ScanPcc File1.Path, File1.filename
  Open LbFitx.Caption For Binary Access Read As #1 Len = 12000
  For i = 0 To Lst.ListCount - 1
    f = Lst.List(i)
    ext = UCase(Right(f, 3))
    Select Case ext
    Case "PAC", "ASS", "PUZ", "SOP", "CRW", "TXA", "TXT"
      DesCompFile i + 1, App.Path & "\res.txt"
      If ext = "PAC" Then
        ScanFilePac File1.Path, f
      ElseIf ext = "TXT" Then
        ScanFileTxt File1.Path, getRelativePath(f), f
      Else
        ScanFileAct File1.Path, f
      End If
    Case Else
    End Select
  Next i
  Close 1
  lbMsgScan = "FET!"

End Sub
Sub scanFile(fp As String, rp As String, l As String)

Dim s As String
Dim i As Integer
Dim nohies As Boolean

If l = "" Then Exit Sub

lbMsgScan = "Analitzant " & l

s = UCase(simplifyPath(rp & l))
For i = 0 To Lst.ListCount - 1
  If UCase(Lst.List(i)) = s Then
    Exit Sub
  End If
Next i

nohies = False
On Error GoTo salta
i = GetAttr(fp & "\" & s)
On Error GoTo 0

If nohies Then
  lsterr.AddItem "NO ES TROBA " & s
Else
  'Debug.Print s & " no és a la llista"
End If
Exit Sub

salta:
nohies = True
Resume Next

End Sub
Sub ScanPcc(fp$, fn$)
  
  Dim pcc As PccEntry
  Dim v$
  Dim i As Integer
  
  LbCont.Caption = "Fitxers compactats a:"
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
    MsgBox "Error: Aquest fitxer no ha estat generat amb ClicFile 2.1"
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
    'nfx = nfx + 1
    'Select Case UCase$(Right$(pcc.name, 4))
    '  Case ".PAC"
    '    npqx = npqx + 1
    '  Case ".ASS", ".PUZ", ".SOP", ".CRW", ".TXA"
    '    nactx = nactx + 1
    'End Select
    'UpdCaption
    DoEvents
  Next i
  
  Close #1
  
Exit Sub
DetectaErrorSCPCC:
MissatgeError fn$
Resume Next

End Sub


Sub NetejaLlistaCompClic()

  Lst.Clear
  LbFitx.Caption = ""
  LbCont.Caption = ""
  Crides = 0
  DoEvents
  
End Sub

Sub ScanFilePac(fp$, fn$)

Dim l$, rp$
Dim Ver As Integer, i As Integer, h As Integer

h = 2
Open App.Path & "\res.txt" For Input As #2
If CANCEL Then Exit Sub

rp$ = getRelativePath(fn$)

On Error GoTo DetectaErrorSFPC

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
        scanFile fp$, rp$, l$
      Next i
      For i = 1 To 4: Line Input #h, l$: Next i
    End If
  End If
  'Fitxers del paquet
  Do While Not EOF(h)
    Line Input #h, l$
    scanFile fp$, rp$, l$
  Loop

Close 2

Exit Sub
DetectaErrorSFPC:
MissatgeError fn$
Resume Next

End Sub

Private Sub Form_Load()
ReDim LastRef$(0 To 200)
End Sub

Sub ScanFileTxt(fp$, rp$, fn$)

Dim i As Integer, k As Integer, h As Integer
Dim l$, ch$

h = 2
Open App.Path & "\res.txt" For Input As #2
LastRef$(Crides) = fn$

  Do While Not EOF(h)
    Line Input #h, l$
    ScanTxLin fp$, rp$, l$
  Loop
  
Close h

Exit Sub
DetectaErrorSFT:
MissatgeError fn$
Resume Next

End Sub

Sub ScanTxLin(fp$, rp$, l$)

Dim i As Integer, k As Integer, ch$

If l$ <> "" Then
  For i = 1 To Len(l$)
    ch$ = Mid$(l$, i, 1)
    If ch$ = "{" Then
      For k = i + 1 To Len(l$)
        ch$ = Mid$(l$, k, 1)
        If ch$ = "}" Or ch$ = " " Then
          scanFile fp$, rp$, Mid$(l$, i + 1, k - i - 1)
          Exit For
        End If
      Next k
      i = k
    End If
  Next i
End If

End Sub

Sub ScanFileAct(fp$, fn$)

Dim l$, a$, rp$
Dim Ver As Integer, i As Integer, h As Integer, nl As Integer, nc As Integer

If CANCEL Then Exit Sub

rp$ = getRelativePath(fn$)
h = 2
Open App.Path & "\res.txt" For Input As #2
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
      scanFile fp$, rp$, l$
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
      Line Input #h, l$: scanFile fp$, rp$, l$
    End If
    
    'Fitxer DLL
    If Ver > 114 Then
      Line Input #h, l$
      Line Input #h, l$: scanFile fp$, rp$, l$
      Line Input #h, l$
    End If

    Select Case UCase$(Right$(fn$, 4))
      Case ".PUZ"
        Line Input #h, l$: scanFile fp$, rp$, l$
        ' venen més coses... però no són fitxers
        
      Case ".ASS"
        If Ver > 109 Then Line Input #h, l$
        Line Input #h, l$
        Line Input #h, l$
        Line Input #h, l$: scanFile fp$, rp$, l$
        Line Input #h, l$: scanFile fp$, rp$, l$
        Line Input #h, l$
        Line Input #h, l$
        If Ver > 106 Then
          Line Input #h, l$
          If Val(l$) <> 0 Then
            Line Input #h, l$: scanFile fp$, rp$, l$
          End If
        End If
        ' venen més coses... però no són fitxers
        
      Case ".SOP"
        Line Input #h, l$:
        If l$ <> "" Then
          If Left$(l$, 1) <> "*" Then scanFile fp$, rp$, l$
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

Exit Sub
DetectaError:
MissatgeError fn$
Resume Next

End Sub

