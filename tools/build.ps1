param(
  [Parameter(Mandatory = $true)][string]$BorlandRoot,
  [string]$WindowsSdkRc = ''
)

$ErrorActionPreference = 'Stop'
$projectRoot = Split-Path -Parent $PSScriptRoot
$borlandRoot = (Resolve-Path -LiteralPath $BorlandRoot).Path
$compiler = Join-Path $borlandRoot 'bin\bcc32.exe'
$include = Join-Path $borlandRoot 'include'
$library = Join-Path $borlandRoot 'lib'
$bin = Join-Path $borlandRoot 'bin'

if (-not $WindowsSdkRc) {
  $kitsRoot = Join-Path ${env:ProgramFiles(x86)} 'Windows Kits\10\bin'
  if (-not (Test-Path -LiteralPath $kitsRoot)) {
    throw 'Windows SDK not found. Pass -WindowsSdkRc with path to rc.exe.'
  }
  $sdkVersion = Get-ChildItem -LiteralPath $kitsRoot -Directory | Sort-Object Name -Descending | Select-Object -First 1
  $WindowsSdkRc = Join-Path $sdkVersion.FullName 'x86\rc.exe'
}
$resourceCompiler = (Resolve-Path -LiteralPath $WindowsSdkRc).Path

$source = Join-Path $projectRoot 'src'
$resourceSource = Join-Path $source 'resources'
$build = Join-Path $projectRoot 'build'
$objects = Join-Path $build 'obj'
$dist = Join-Path $projectRoot 'dist'
$resourceFile = Join-Path $build 'clic.res'

New-Item -ItemType Directory -Force -Path $objects, $dist | Out-Null

& (Join-Path $PSScriptRoot 'prepare-branding.ps1')

Push-Location $resourceSource
try {
  & $resourceCompiler /nologo /D NOEDIT /D RUNTIME /D RC_INVOKED /I $include /fo $resourceFile 'clic.rc'
  if ($LASTEXITCODE -ne 0) { throw "Resource compilation failed: $LASTEXITCODE" }
}
finally {
  Pop-Location
}

$sourceFiles = @(
  'clic.cpp', 'playwin.cpp', 'actfile.cpp', 'clicdlg.cpp', 'zones.cpp',
  'casella.cpp', 'report.cpp', 'miscfunc.cpp', 'ctext.cpp', 'floatcnt.cpp',
  'cmpstr.cpp', 'winbase.cpp', 'bitmap.cpp', 'wgiffile.cpp', 'mcifuncs.cpp', 'runtime-association.cpp', 'runtime-i18n.cpp', 'runtime-branding.cpp'
) | ForEach-Object { Join-Path $source $_ }

$compileArgs = @(
  '-W', '-M', '-K', '-DNOEDIT', '-DRUNTIME',
  ('-I' + $source), ('-I' + $include), ('-L' + $library),
  ('-n' + $objects), '-eClicPlayer.exe'
) + $sourceFiles

$oldPath = $env:PATH
try {
  $env:PATH = $bin + ';' + $env:PATH
  Push-Location $build
  try {
    & $compiler @compileArgs
    if ($LASTEXITCODE -ne 0) { throw "Borland C++ compilation failed: $LASTEXITCODE" }
  }
  finally {
    Pop-Location
  }
}
finally {
  $env:PATH = $oldPath
}

$builtExe = Join-Path $objects 'ClicPlayer.exe'
& (Join-Path $PSScriptRoot 'embed-resources.ps1') -Executable $builtExe -ResourceFile $resourceFile
Copy-Item -LiteralPath $builtExe -Destination (Join-Path $dist 'ClicPlayer.exe') -Force
Write-Output "Built $(Join-Path $dist 'ClicPlayer.exe')"
