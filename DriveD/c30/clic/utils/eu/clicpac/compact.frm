VERSION 2.00
Begin Form Compact 
   BackColor       =   &H00C0C0C0&
   BorderStyle     =   1  'Fixed Single
   Caption         =   "Trinkotu"
   ClientHeight    =   4365
   ClientLeft      =   375
   ClientTop       =   1095
   ClientWidth     =   8910
   ControlBox      =   0   'False
   Height          =   4770
   HelpContextID   =   100
   Icon            =   COMPACT.FRX:0000
   Left            =   315
   LinkTopic       =   "Form1"
   MaxButton       =   0   'False
   MinButton       =   0   'False
   ScaleHeight     =   4365
   ScaleWidth      =   8910
   Top             =   750
   Width           =   9030
   Begin CommandButton BtnExcl2 
      BackColor       =   &H00C0C0C0&
      Caption         =   ">> ..\xx >>"
      Height          =   375
      Left            =   2400
      TabIndex        =   13
      Top             =   2160
      Width           =   1455
   End
   Begin CommandButton BtnAjuda 
      BackColor       =   &H00C0C0C0&
      Caption         =   "Laguntza"
      Height          =   495
      Left            =   6300
      TabIndex        =   12
      Top             =   3720
      Width           =   2475
   End
   Begin TextBox TxCom 
      BackColor       =   &H00FFFFFF&
      Height          =   1335
      Left            =   6300
      MaxLength       =   200
      MultiLine       =   -1  'True
      TabIndex        =   10
      Top             =   600
      Width           =   2475
   End
   Begin CommandButton BtnIncloure 
      BackColor       =   &H00C0C0C0&
      Caption         =   "<< Onartu <<"
      Height          =   375
      Left            =   2400
      TabIndex        =   9
      Top             =   1440
      Width           =   1455
   End
   Begin CommandButton BtnExcloure 
      BackColor       =   &H00C0C0C0&
      Caption         =   ">> Baztertu >>"
      Height          =   375
      Left            =   2400
      TabIndex        =   8
      Top             =   960
      Width           =   1455
   End
   Begin CommandButton BtnCompactar 
      BackColor       =   &H00C0C0C0&
      Caption         =   "Pakete trinkoa sortu"
      Default         =   -1  'True
      Height          =   495
      Left            =   6300
      TabIndex        =   7
      Top             =   2280
      Width           =   2475
   End
   Begin TextBox TxFname 
      BackColor       =   &H00FFFFFF&
      Height          =   285
      Left            =   240
      TabIndex        =   3
      Top             =   3960
      Width           =   5895
   End
   Begin ListBox LstNoCmp 
      BackColor       =   &H00FFFFFF&
      Height          =   2955
      Left            =   3960
      MultiSelect     =   2  'Extended
      TabIndex        =   2
      Top             =   600
      Width           =   2175
   End
   Begin ListBox LstCmp 
      BackColor       =   &H00FFFFFF&
      Height          =   2955
      Left            =   240
      MultiSelect     =   2  'Extended
      TabIndex        =   1
      Top             =   600
      Width           =   2055
   End
   Begin CommandButton BtnCancel 
      BackColor       =   &H00C0C0C0&
      Caption         =   "Utzi"
      Height          =   495
      Left            =   6300
      TabIndex        =   0
      Top             =   3000
      Width           =   2475
   End
   Begin Label LbExcl2 
      BackColor       =   &H00C0C0C0&
      Caption         =   "Paketearen direktorioan ez daudenak baztertu"
      FontBold        =   0   'False
      FontItalic      =   0   'False
      FontName        =   "MS Sans Serif"
      FontSize        =   8.25
      FontStrikethru  =   0   'False
      FontUnderline   =   0   'False
      Height          =   915
      Left            =   2400
      TabIndex        =   14
      Top             =   2580
      Width           =   1455
   End
   Begin Label Label4 
      BackColor       =   &H00C0C0C0&
      Caption         =   "Deskribapena:"
      Height          =   255
      Left            =   6300
      TabIndex        =   11
      Top             =   360
      Width           =   2415
   End
   Begin Label Label3 
      BackColor       =   &H00C0C0C0&
      Caption         =   "Pakete trinkoa:"
      Height          =   255
      Left            =   240
      TabIndex        =   6
      Top             =   3720
      Width           =   4995
   End
   Begin Label Label2 
      BackColor       =   &H00C0C0C0&
      Caption         =   "Paketearen trinkoan baztertutako fitxategiak:"
      Height          =   495
      Left            =   3960
      TabIndex        =   5
      Top             =   120
      Width           =   2175
   End
   Begin Label Label1 
      BackColor       =   &H00C0C0C0&
      Caption         =   "Pakete trinkoan onartutako fitxategiak:"
      Height          =   495
      Left            =   240
      TabIndex        =   4
      Top             =   120
      Width           =   2055
   End
End
Option Explicit

Sub BtnAjuda_Click ()

  SendKeys "{F1}"

End Sub

Private Sub BtnCancel_Click ()

CANCEL = True
Tanca

End Sub

Private Sub BtnCompactar_Click ()

  If LstCmp.ListCount = 0 Then
    MsgBox "Ez da trinkotzeko paketerik hautatu!"
    Exit Sub
  End If

  CANCEL = False
  CDLG.LB1 = "Trinkotzen ari da..."
  CDLG.LB2 = ""
  CDLG.Show
  popupOn = True
  DoEvents

  Compacta
  
  popupOn = False
  CDLG.Hide
  Close

  Tanca

End Sub

Sub BtnExcl2_Click ()

Dim i As Integer

i = 0
Do While i < LstCmp.ListCount
  If InStr(LstCmp.List(i), "\") <> 0 Then
    If i = 0 Then
      MsgBox "Fitxategi hau: " + LstCmp.List(i) + " pakete nagusia da, eta ezin da zerrendatik ezabatu."
    Else
      LstNoCmp.AddItem LstCmp.List(i)
      LstCmp.RemoveItem i
      i = i - 1
    End If
  End If
  i = i + 1
Loop

End Sub

Private Sub BtnExcloure_Click ()

Dim i As Integer

i = 0
Do While i < LstCmp.ListCount
  If LstCmp.Selected(i) = True Then
    If i = 0 Then
      MsgBox "Fitxategi hau: " + LstCmp.List(i) + " pakete nagusia da, eta ezin da zerrendatik ezabatu."
    Else
      LstNoCmp.AddItem LstCmp.List(i)
      LstCmp.RemoveItem i
      i = i - 1
    End If
  End If
  i = i + 1
Loop

BtnExcloure.Enabled = False

End Sub

Private Sub BtnIncloure_Click ()

Dim i As Integer
Dim a$

i = 0
Do While i < LstNoCmp.ListCount
  If LstNoCmp.Selected(i) = True Then
    a$ = LstNoCmp.List(i)
    Select Case UCase$(Right$(a$, 3))
      Case "ASS", "PUZ", "SOP", "CRW", "PAC", "TXT", "BMP", "GIF", "TXA"
        LstCmp.AddItem LstNoCmp.List(i)
        LstNoCmp.RemoveItem i
        i = i - 1
      Case Else
        MsgBox "Fitxategi hau: " + a$ + " ezin da pakete trinkoan onartu. Soilik onartzen dira ASS, PUZ, CRW, SOP, TXA, PAC, TXT, BMP y GIF motakoak"
    End Select
  End If
  i = i + 1
Loop

BtnIncloure.Enabled = False

End Sub

Sub Compacta ()

Dim fn$, fn2$, a$, r$, fidesc$, d$
Dim i As Integer, n As Integer, k As Integer
Dim l As Long, l2 As Long, lcap As Long, b As String * 1
Dim s As String

fn$ = TxFname

If fn$ = "" Or Len(fn$) < 5 Or UCase$(Right(fn$, 3)) <> "PCC" Then
  MsgBox "Errore pakete trinkoaren izenean!"
  Exit Sub
End If

If Not OverTest(fn$) Then Exit Sub

CDLG.LB1 = "Trinkotzen, nora? " + fn$ + ":"
CDLG.LB2 = "Fitxategiaren tamaina kalkulatzen ari da."
DoEvents

'Estructura del fitxer compactat:
'Capçalera:
'  Versió (PCC00) - 5 bytes
'  Retorn (0x0D 0x0A)
'  Descripció ('k' bytes)
'  Fi de descripció (0x87 0x0D 0x0A)
'  Número d'entrades (2 bytes)
'Diccionari:
'  Nom (string acabat amb zero)
'  Offset fitxer (4 bytes)
'  Longitud fitxer (4 bytes)
'Dades

If TxCom = "" Then k = 0 Else k = Len(TxCom)
lcap = 5 + 2 + k + 3 + 2 + 4
n = LstCmp.ListCount
For i = 1 To n
  lcap = lcap + Len(LstCmp.List(i - 1)) + (1 + 4 + 4)
Next i

On Error GoTo DetectaError
Open fn$ For Binary Access Write As #1 Len = 12000
If CANCEL Then Exit Sub
On Error GoTo 0

a$ = "PCC00"
r$ = Chr$(&HD) + Chr$(&HA)
Put #1, , a$
Put #1, , r$
If (k > 0) Then
  d$ = TxCom.Text
  Put #1, , d$
End If
fidesc$ = Chr$(&H87)
Put #1, , fidesc$
Put #1, , r$
Put #1, , n
Put #1, , lcap

l2 = lcap
For i = 1 To n
  If CANCEL Then Exit For
  fn2$ = LstCmp.List(i - 1)
  Put #1, , fn2$
  b = Chr$(0)
  Put #1, , b
  Put #1, , l2
  fn$ = CompClic.File1.Path + "\" + fn2$
  On Error GoTo DetectaError
  l = FileLen(fn$)
  If CANCEL Then Exit Sub
  On Error GoTo 0
  Put #1, , l
  l2 = l2 + l
  DoEvents
Next i

For i = 1 To n
  If CANCEL Then Exit For
  fn$ = CompClic.File1.Path + "\" + LstCmp.List(i - 1)
  CDLG.LB2 = fn$
  DoEvents
  On Error GoTo DetectaError
  Open fn$ For Binary Access Read As #2 Len = 12000
  If CANCEL Then Exit Sub
  On Error GoTo 0
    l = LOF(2)
    Do While l > 0 And CANCEL = False
      If l > 12000 Then l2 = 12000 Else l2 = l
      s = Input(l2, #2)
      Put #1, , s
      l = l - l2
      DoEvents
    Loop
  Close #2
Next i

Close

k = MsgBox(Str$(n) + " Fitxategiak trinkotuta errorerik gabe." + ret$ + "Ezabatu nahi dituzue fitxategi hauek, behin pakete trinkoan daudela?", 4 + 32)
If k = 6 Then
  CDLG.LB1 = "Fitxategia ezabatzen"
  On Error GoTo DetectaError
  For i = 1 To n
    fn$ = CompClic.File1.Path + "\" + LstCmp.List(i - 1)
    CDLG.LB2 = fn$
    DoEvents
    Kill fn$
  Next i
  On Error GoTo 0
End If

Exit Sub
DetectaError:
MissatgeError fn$
Resume Next

End Sub

Private Sub Form_Activate ()

  If popupOn Then
    CDLG.SetFocus
    Exit Sub
  End If
  
End Sub

Sub Form_Load ()

  Left = (screen.Width - Width) / 2
  Top = (screen.Height - Height) / 2
  Inicialitza

End Sub

Sub Inicialitza ()

Dim a$, b$, fn$
Dim n As Integer, i As Integer, j As Integer, v As Integer

LstCmp.Clear
LstNoCmp.Clear
TxCom = ""

a$ = CompClic.LbFitx
If a$ = "" Then
  MsgBox "Trinkotzeko paketerik ez da hautatu!"
  Tanca
  Exit Sub
End If
If UCase$(Right$(a$, 3)) <> "PAC" Then
  MsgBox "Trinkotzeko, pakete bat (*.PAC fitxategia) hautatu behar duzue"
  Tanca
  Exit Sub
End If

fn$ = a$
On Error GoTo DetectaErrorINI
Open fn$ For Input As #1
If CANCEL Then Exit Sub
On Error GoTo 0
  Line Input #1, b$
  v = Val(b$)
  Select Case v
    Case Is < 116
      MsgBox "ERROREA: Pakete hau Kliken bertsio zahar batez sortua da."
      Tanca
      Close #1
      Exit Sub
    
    Case Is > 120
      MsgBox "ERROREA: Pakete hau Kliken 3.0 baino berriagoa den bertsio batez sortua izan da"
      Tanca
      Exit Sub
      
    Case Else
      Do While Not EOF(1)
       Line Input #1, b$
       If b$ <> "" Then
         If Asc(Left$(b$, 1)) = &H87 Then Exit Do
         If TxCom <> "" Then TxCom = TxCom + Chr$(&HD) + Chr$(&HA)
         TxCom = TxCom + b$
       End If
      Loop
   End Select

Close #1

TxFname.Text = Left$(a$, Len(a$) - 3) + "PCC"

n = CompClic.Lst.ListCount
If n < 1 Then
  MsgBox "Pakete honetan ez dago ezer!"
  Tanca
  Exit Sub
End If

j = False
For i = 0 To n - 1
  a$ = CompClic.Lst.List(i)
  Select Case UCase$(Right$(a$, 3))
    Case "ASS", "PUZ", "SOP", "CRW", "PAC", "TXT", "BMP", "GIF", "TXA"
      LstCmp.AddItem a$
      If j = False Then
        If InStr(a$, "\") <> 0 Then j = True
      End If
    Case Else
      LstNoCmp.AddItem a$
  End Select
Next i
    
BtnExcl2.Visible = j
LbExcl2.Visible = j
BtnExcl2.Enabled = j

BtnExcloure.Enabled = False
BtnIncloure.Enabled = False

Exit Sub
DetectaErrorINI:
MissatgeError fn$
Resume Next

End Sub

Private Sub LstCmp_Click ()

  BtnExcloure.Enabled = True

End Sub

Private Sub LstNoCmp_Click ()

  BtnIncloure.Enabled = True

End Sub

Sub Tanca ()
  
  Hide
  cmpOn = False
  Close
  NetejaLlistaCompClic
  Unload Compact

End Sub

