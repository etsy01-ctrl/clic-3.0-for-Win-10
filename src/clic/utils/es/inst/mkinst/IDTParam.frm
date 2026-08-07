VERSION 2.00
Begin Form IDTParam 
   BackColor       =   &H00C0C0C0&
   BorderStyle     =   1  'Fixed Single
   Caption         =   "MKInst 1.0 - Parámetros de instalación"
   ClientHeight    =   6135
   ClientLeft      =   1635
   ClientTop       =   2085
   ClientWidth     =   7365
   ControlBox      =   0   'False
   Height          =   6540
   Icon            =   IDTPARAM.FRX:0000
   Left            =   1575
   LinkTopic       =   "Form1"
   MaxButton       =   0   'False
   MinButton       =   0   'False
   ScaleHeight     =   6135
   ScaleWidth      =   7365
   Top             =   1740
   Width           =   7485
   Begin CommandButton BtnCancel 
      Caption         =   "Cancelar"
      Height          =   435
      Left            =   4080
      TabIndex        =   1
      Top             =   5640
      Width           =   1875
   End
   Begin CommandButton BtnAcceptar 
      Caption         =   "Aceptar"
      Height          =   435
      Left            =   1320
      TabIndex        =   8
      Top             =   5640
      Width           =   1875
   End
   Begin Frame Frame2 
      BackColor       =   &H00C0C0C0&
      Caption         =   "Icono adicional:"
      Height          =   1995
      Left            =   120
      TabIndex        =   22
      Top             =   3540
      Width           =   7095
      Begin TextBox txFile2 
         Enabled         =   0   'False
         Height          =   285
         Left            =   120
         TabIndex        =   27
         Top             =   900
         Width           =   1935
      End
      Begin TextBox txDesc2 
         Height          =   285
         Left            =   4200
         TabIndex        =   26
         Top             =   900
         Width           =   2655
      End
      Begin TextBox txIco2 
         Enabled         =   0   'False
         Height          =   285
         Left            =   120
         TabIndex        =   25
         Top             =   1500
         Width           =   1935
      End
      Begin CommandButton btnCercaFile2 
         Caption         =   "Buscar..."
         FontBold        =   0   'False
         FontItalic      =   0   'False
         FontName        =   "MS Sans Serif"
         FontSize        =   8.25
         FontStrikethru  =   0   'False
         FontUnderline   =   0   'False
         Height          =   315
         Left            =   2160
         TabIndex        =   24
         Top             =   900
         Width           =   1215
      End
      Begin CommandButton btnCercaIco2 
         Caption         =   "Buscar..."
         FontBold        =   0   'False
         FontItalic      =   0   'False
         FontName        =   "MS Sans Serif"
         FontSize        =   8.25
         FontStrikethru  =   0   'False
         FontUnderline   =   0   'False
         Height          =   315
         Left            =   2160
         TabIndex        =   23
         Top             =   1500
         Width           =   1215
      End
      Begin Label Label4 
         BackStyle       =   0  'Transparent
         Caption         =   "Texto breve que acompaña al icono"
         FontBold        =   0   'False
         FontItalic      =   0   'False
         FontName        =   "MS Sans Serif"
         FontSize        =   8.25
         FontStrikethru  =   0   'False
         FontUnderline   =   0   'False
         Height          =   255
         Left            =   4200
         TabIndex        =   31
         Top             =   1260
         Width           =   2835
      End
      Begin Label Label16 
         BackStyle       =   0  'Transparent
         Caption         =   "Archivo:"
         Height          =   255
         Left            =   120
         TabIndex        =   14
         Top             =   660
         Width           =   1695
      End
      Begin Label Label15 
         BackStyle       =   0  'Transparent
         Caption         =   "Descripción:"
         Height          =   255
         Left            =   4200
         TabIndex        =   28
         Top             =   660
         Width           =   1935
      End
      Begin Label Label14 
         BackStyle       =   0  'Transparent
         Caption         =   "El instalador puede crear un segundo icono que apunte a otro archivo. Si no necesita este segundo icono puede dejar los campos en blanco."
         FontBold        =   0   'False
         FontItalic      =   0   'False
         FontName        =   "MS Sans Serif"
         FontSize        =   8.25
         FontStrikethru  =   0   'False
         FontUnderline   =   0   'False
         Height          =   555
         Left            =   120
         TabIndex        =   29
         Top             =   240
         Width           =   6855
      End
      Begin Label Label13 
         BackStyle       =   0  'Transparent
         Caption         =   "Archivo ICO:"
         Height          =   255
         Left            =   120
         TabIndex        =   30
         Top             =   1260
         Width           =   1935
      End
   End
   Begin Frame Frame1 
      BackColor       =   &H00C0C0C0&
      Caption         =   "Icono principal:"
      Height          =   2115
      Left            =   120
      TabIndex        =   9
      Top             =   1320
      Width           =   7095
      Begin CommandButton btnCercaIco1 
         Caption         =   "Buscar..."
         FontBold        =   0   'False
         FontItalic      =   0   'False
         FontName        =   "MS Sans Serif"
         FontSize        =   8.25
         FontStrikethru  =   0   'False
         FontUnderline   =   0   'False
         Height          =   315
         Left            =   2160
         TabIndex        =   21
         Top             =   1440
         Width           =   1215
      End
      Begin CommandButton btnCercaFile1 
         Caption         =   "Buscar..."
         FontBold        =   0   'False
         FontItalic      =   0   'False
         FontName        =   "MS Sans Serif"
         FontSize        =   8.25
         FontStrikethru  =   0   'False
         FontUnderline   =   0   'False
         Height          =   315
         Left            =   2160
         TabIndex        =   20
         Top             =   540
         Width           =   1215
      End
      Begin TextBox txIco1 
         Enabled         =   0   'False
         Height          =   285
         Left            =   120
         TabIndex        =   17
         Top             =   1440
         Width           =   1935
      End
      Begin TextBox txDesc1 
         Height          =   285
         Left            =   4200
         TabIndex        =   13
         Top             =   540
         Width           =   2655
      End
      Begin TextBox txFile1 
         Enabled         =   0   'False
         Height          =   285
         Left            =   120
         TabIndex        =   11
         Top             =   540
         Width           =   1935
      End
      Begin Label Label10 
         BackStyle       =   0  'Transparent
         Caption         =   "Archivo PCC o PAC principal"
         FontBold        =   0   'False
         FontItalic      =   0   'False
         FontName        =   "MS Sans Serif"
         FontSize        =   8.25
         FontStrikethru  =   0   'False
         FontUnderline   =   0   'False
         Height          =   255
         Left            =   120
         TabIndex        =   19
         Top             =   900
         Width           =   3375
      End
      Begin Label Label9 
         BackStyle       =   0  'Transparent
         Caption         =   "Texto breve que acompaña al icono"
         FontBold        =   0   'False
         FontItalic      =   0   'False
         FontName        =   "MS Sans Serif"
         FontSize        =   8.25
         FontStrikethru  =   0   'False
         FontUnderline   =   0   'False
         Height          =   255
         Left            =   4200
         TabIndex        =   18
         Top             =   900
         Width           =   2835
      End
      Begin Label Label8 
         BackStyle       =   0  'Transparent
         Caption         =   "Archivo ICO:"
         Height          =   255
         Left            =   120
         TabIndex        =   16
         Top             =   1200
         Width           =   1935
      End
      Begin Label Label7 
         BackStyle       =   0  'Transparent
         Caption         =   "Opcional: si deja este campo en blanco se mostrará el icono de Clic"
         FontBold        =   0   'False
         FontItalic      =   0   'False
         FontName        =   "MS Sans Serif"
         FontSize        =   8.25
         FontStrikethru  =   0   'False
         FontUnderline   =   0   'False
         Height          =   255
         Left            =   120
         TabIndex        =   15
         Top             =   1800
         Width           =   6855
      End
      Begin Label Label6 
         BackStyle       =   0  'Transparent
         Caption         =   "Descripción:"
         Height          =   255
         Left            =   4200
         TabIndex        =   12
         Top             =   300
         Width           =   1935
      End
      Begin Label Label5 
         BackStyle       =   0  'Transparent
         Caption         =   "Paquete de actividades:"
         Height          =   255
         Left            =   120
         TabIndex        =   10
         Top             =   300
         Width           =   2355
      End
   End
   Begin CommandButton btnCercarCarpeta 
      Caption         =   "Buscar..."
      FontBold        =   0   'False
      FontItalic      =   0   'False
      FontName        =   "MS Sans Serif"
      FontSize        =   8.25
      FontStrikethru  =   0   'False
      FontUnderline   =   0   'False
      Height          =   315
      Left            =   6120
      TabIndex        =   7
      Top             =   840
      Width           =   1095
   End
   Begin TextBox txFolder 
      Enabled         =   0   'False
      Height          =   285
      Left            =   3180
      TabIndex        =   6
      Top             =   840
      Width           =   2895
   End
   Begin TextBox txAutors 
      Height          =   285
      Left            =   3180
      TabIndex        =   4
      Top             =   480
      Width           =   4035
   End
   Begin TextBox txTitol 
      Height          =   285
      Left            =   3180
      TabIndex        =   2
      Top             =   120
      Width           =   4035
   End
   Begin Label Label3 
      Alignment       =   1  'Right Justify
      BackStyle       =   0  'Transparent
      Caption         =   "Carpeta en la que se encuentra el paquete de actividades:"
      Height          =   435
      Left            =   60
      TabIndex        =   5
      Top             =   780
      Width           =   3075
   End
   Begin Label Label2 
      Alignment       =   1  'Right Justify
      BackStyle       =   0  'Transparent
      Caption         =   "Autor/a o autores/as:"
      Height          =   255
      Left            =   60
      TabIndex        =   3
      Top             =   480
      Width           =   3075
   End
   Begin Label Label1 
      Alignment       =   1  'Right Justify
      BackStyle       =   0  'Transparent
      Caption         =   "Título del paquete de actividades:"
      Height          =   255
      Left            =   60
      TabIndex        =   0
      Top             =   120
      Width           =   3075
   End
End
Option Explicit

Sub BtnAcceptar_Click ()

Dim folder As String
Dim baseFolder As String
Dim i As Integer

If txFolder = "" Then
  MsgBox "¡Debe indicar la carpeta en la que se encuentra el paquete de actividades!"
  Exit Sub
End If

For i = Len(txFolder) To 1 Step -1
  If Mid(txFolder, i, 1) = "\" Then Exit For
Next i
baseFolder = Left(txFolder, i - 1)
folder = Right(txFolder, Len(txFolder) - i)
srcFolder = txFolder

If txTitol = "" Then
  MsgBox "¡Hay que indicar el título del paquete de actividades!"
  Exit Sub
End If
If txAutors = "" Then
  MsgBox "¡Hay que llenar el campo de autor/es!"
  Exit Sub
End If
If txFile1 = "" Then
  MsgBox "Hay que indicar el nombre del archivo del paquete de actividades!"
  Exit Sub
End If
If txDesc1 = "" Then
  MsgBox "¡Hay que escribir una descripción para el icono del paquete de actividades!"
  Exit Sub
End If

If txDesc2 = "" And txFile2 <> "" Then
  MsgBox "¡Hay que escribir una descripción para el icono adicional!"
  Exit Sub
End If

On Error GoTo errWrite
Open tempFile For Output As #1
Print #1, "[instal]"
Print #1, "language=" & "spanish"
Print #1, "title=" & txTitol
Print #1, "authors=" & txAutors
Print #1, "folder=" & folder
Print #1, "icon1.file=" & txFile1
Print #1, "icon1.description=" & txDesc1
If txIco1 <> "" Then
  Print #1, "icon1.icon=" & txIco1
End If
If txFile2 <> "" Then
  Print #1, "icon2.file=" & txFile2
  Print #1, "icon2.description=" & txDesc2
  If txIco2 <> "" Then
    Print #1, "icon2.icon=" & txIco2
  End If
End If
Close 1
dlgResult = baseFolder

cancel = False
Unload IDTParam
Exit Sub

errWrite:
If MostraError(tempFile) = 2 Then Exit Sub
Resume

End Sub

Sub BtnCancel_Click ()

cancel = True
Unload IDTParam

End Sub

Sub btnCercaFile1_Click ()

  cancel = False
  If txFolder.Text = "" Then
    MsgBox "¡Seleccione antes la carpeta donde se encuentra el paquete de actividades!"
    Exit Sub
  End If
  
  dlgResult = txFolder.Text
  dlgParam = "*.PCC;*.PAC"
  
  SFile.Show 1
  If cancel = False Then
    txFile1.Text = dlgResult
  End If

End Sub

Sub btnCercaFile2_Click ()

  cancel = False
  If txFolder.Text = "" Then
    MsgBox "¡Seleccione antes la carpeta donde se encuentra el paquete de actividades!"
    Exit Sub
  End If
  
  dlgResult = txFolder.Text
  dlgParam = "*.*"
  
  SFile.Show 1
  If cancel = False Then
    txFile2.Text = dlgResult
  End If

End Sub

Sub btnCercaIco1_Click ()

  cancel = False
  If txFolder.Text = "" Then
    MsgBox "¡Seleccione antes la carpeta donde se encuentra el paquete de actividades!"
    Exit Sub
  End If
  
  dlgResult = txFolder.Text
  dlgParam = "*.ICO"
  
  SFile.Show 1
  If cancel = False Then
    txIco1.Text = dlgResult
  End If

End Sub

Sub btnCercaIco2_Click ()
  
  cancel = False
  If txFolder.Text = "" Then
    MsgBox "¡Seleccione antes la carpeta donde se encuentra el paquete de actividades!"
    Exit Sub
  End If
  
  dlgResult = txFolder.Text
  dlgParam = "*.ICO"
  
  SFile.Show 1
  If cancel = False Then
    txIco2.Text = dlgResult
  End If

End Sub

Sub btnCercarCarpeta_Click ()

  cancel = False
  SFolder.Show 1
  If cancel = False Then
    If txFolder.Text <> dlgResult Then
      txFile1.Text = ""
      txDesc1.Text = ""
      txFile2.Text = ""
      txDesc2.Text = ""
      txIco1.Text = ""
      txIco2.Text = ""
    End If
    txFolder.Text = dlgResult
  End If

End Sub

Sub Form_Load ()

centrarImatge IDTParam

End Sub

