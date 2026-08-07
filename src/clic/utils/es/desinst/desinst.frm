VERSION 2.00
Begin Form DesInst 
   BackColor       =   &H00C0C0C0&
   Caption         =   "Desinstalador de actividades Clic"
   ClientHeight    =   4125
   ClientLeft      =   660
   ClientTop       =   1035
   ClientWidth     =   5715
   Height          =   4530
   Icon            =   DESINST.FRX:0000
   Left            =   600
   LinkTopic       =   "Form1"
   ScaleHeight     =   4125
   ScaleWidth      =   5715
   Top             =   690
   Width           =   5835
   Begin CommandButton BtnCancel 
      Caption         =   "Cancelar"
      Height          =   495
      Left            =   3000
      TabIndex        =   3
      Top             =   3480
      Width           =   1935
   End
   Begin CommandButton BtnDesinst 
      Caption         =   "Desinstalar"
      Enabled         =   0   'False
      Height          =   495
      Left            =   600
      TabIndex        =   2
      Top             =   3480
      Width           =   1935
   End
   Begin ListBox Lst 
      Height          =   2760
      Left            =   120
      TabIndex        =   0
      Top             =   600
      Width           =   5415
   End
   Begin Label Label1 
      BackStyle       =   0  'Transparent
      Caption         =   "Selección del paquete de actividades a desinstalar:"
      Height          =   255
      Left            =   120
      TabIndex        =   1
      Top             =   240
      Width           =   5535
   End
End
Option Explicit

Const ATTR_DIRECTORY = 16
Dim numPacs As Integer
Dim pacDir() As String
Dim pacDesc() As String

Sub BtnCancel_Click ()

  End

End Sub

Sub BtnDesinst_Click ()

  Dim i

  If lst.ListIndex >= 0 Then
    i = Shell(app.Path & "\unwise.exe " & pacDir(lst.ListIndex + 1))
    End
  End If


End Sub

Sub Form_Load ()

  CANCEL = False
  numPacs = 0
  
  Avis.Show
  DoEvents
  
  ScanPacs app.Path + "\"
  
  Avis.Hide
  If CANCEL Then End

  If numPacs = 0 Then
    MsgBox "No se ha encontrado en el disco duro ninguna aplicación Clic previamente instalada.", 16, "¡ATENCIÓN!"
    End
  End If

End Sub

Sub Lst_Click ()

   BtnDesinst.Enabled = True

End Sub

Sub ScanPacs (Path)

  Dim Trobat As Integer, a$, l$
  Dim Count, D(), i, DirName  ' Declare variables.
  
  
  DoEvents
  If CANCEL Then Exit Sub

  DirName = Dir(Path, ATTR_DIRECTORY) ' Get first directory name.
  'Iterate through PATH, caching all subdirectories in D()
  Do While DirName <> ""
    DoEvents
    If CANCEL Then Exit Sub
    If DirName <> "." And DirName <> ".." Then
      If (GetAttr(Path + DirName) And ATTR_DIRECTORY) = ATTR_DIRECTORY Then
	If (Count Mod 10) = 0 Then
	  ReDim Preserve D(Count + 10)    ' Resize the array.
	End If
	Count = Count + 1   ' Increment counter.
	D(Count) = DirName
      End If
    End If
    DirName = Dir   ' Get another directory name.
  Loop
    ' Now recursively iterate through each cached subdirectory.
  For i = 1 To Count
    DoEvents
    If CANCEL Then Exit Sub
    Trobat = True
    On Error GoTo SaltaErr
    a$ = Path & D(i) & "\install.log"
    DirName = Dir(a$)
    If Trobat = True And DirName <> "" Then
      If (numPacs Mod 10) = 0 Then
	ReDim Preserve pacDir(numPacs + 10)    ' Resize the array.
	ReDim Preserve pacDesc(numPacs + 10)    ' Resize the array.
      End If
      numPacs = numPacs + 1   ' Increment counter.
      Open a$ For Input As #1
      Line Input #1, l$
      Line Input #1, l$
      If Left$(l$, 7) = "Title: " Then
	pacDesc(numPacs) = Right$(l$, Len(l$) - 7)
	pacDir(numPacs) = a$
	lst.AddItem pacDesc(numPacs)
      End If
      Close 1
    End If
    ScanPacs Path & D(i) & "\"
  Next i


Exit Sub
SaltaErr:
Trobat = False
Resume Next

End Sub

