$ErrorActionPreference = 'Stop'
$project = Split-Path -Parent $PSScriptRoot
$png = Join-Path $project 'src\resources\assets\cover-branding.png'
$data = Join-Path $project 'src\resources\assets\cover-branding.bgra'
if ((Test-Path -LiteralPath $data) -and
    (Get-Item -LiteralPath $data).LastWriteTimeUtc -ge (Get-Item -LiteralPath $png).LastWriteTimeUtc -and
    (Get-Item -LiteralPath $data).LastWriteTimeUtc -ge (Get-Item -LiteralPath $PSCommandPath).LastWriteTimeUtc) { return }

Add-Type -AssemblyName System.Drawing
$source = [Drawing.Image]::FromFile($png)
$width = [Math]::Min(560, $source.Width)
$height = [int][Math]::Round($source.Height * $width / $source.Width)
$bitmap = New-Object Drawing.Bitmap $width, $height, ([Drawing.Imaging.PixelFormat]::Format32bppPArgb)
$graphics = [Drawing.Graphics]::FromImage($bitmap)
$graphics.CompositingMode = [Drawing.Drawing2D.CompositingMode]::SourceCopy
$graphics.InterpolationMode = [Drawing.Drawing2D.InterpolationMode]::HighQualityBicubic
$graphics.Clear([Drawing.Color]::Transparent)
$graphics.DrawImage($source, 0, 0, $width, $height)
$graphics.Dispose()
$source.Dispose()

$rect = New-Object Drawing.Rectangle 0, 0, $width, $height
$locked = $bitmap.LockBits($rect, [Drawing.Imaging.ImageLockMode]::ReadOnly, [Drawing.Imaging.PixelFormat]::Format32bppPArgb)
$stream = [IO.File]::Create($data)
$writer = New-Object IO.BinaryWriter $stream
try {
  $writer.Write([int]$width)
  $writer.Write([int]$height)
  $row = New-Object byte[] ($width * 4)
  for ($y = 0; $y -lt $height; $y++) {
    [Runtime.InteropServices.Marshal]::Copy([IntPtr]::Add($locked.Scan0, $y * $locked.Stride), $row, 0, $row.Length)
    $writer.Write($row)
  }
}
finally {
  $writer.Dispose()
  $bitmap.UnlockBits($locked)
  $bitmap.Dispose()
}
