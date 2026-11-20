# Arma la carpeta autocontenida de FlatCAM para Windows, un .zip portable y el instalador (Inno Setup).
#
# Uso:  pwsh tools/package_windows.ps1 [-Build build] [-Out build/dist] [-Vcpkg <raíz de vcpkg>]
#       Necesita el build de la GUI (Release), windeployqt (viene con Qt) y, para el instalador, iscc (Inno Setup 6).
#
# Pasos: copia FlatCAM.exe, windeployqt (Qt y sus complementos), las DLL de vcpkg (GEOS, liblzma, FreeType, Tcl y lo que
# arrastran), la biblioteca de scripts de Tcl (FCShell la busca en tcl<versión> al lado del ejecutable), el zip y el
# instalador.
param(
    [string]$Build = "build",
    [string]$Out = "",
    [string]$Vcpkg = $env:VCPKG_INSTALLATION_ROOT,
    [string]$Triplet = "x64-windows"
)
$ErrorActionPreference = "Stop"
$Root = Split-Path -Parent $PSScriptRoot
if (-not $Out) { $Out = Join-Path $Build "dist" }
$exe = Join-Path $Build "src/gui/FlatCAM.exe"
if (-not (Test-Path $exe)) { throw "No está ${exe}: compilá la GUI primero." }
$version = (Select-String -Path (Join-Path $Root "CMakeLists.txt") -Pattern 'VERSION\s+(\d+\.\d+\.\d+)').Matches[0].Groups[1].Value

$app = Join-Path $Out "FlatCAM"
if (Test-Path $app) { Remove-Item -Recurse -Force $app }
New-Item -ItemType Directory -Force $app | Out-Null
Copy-Item $exe $app

Write-Host "windeployqt..."
$deploy = (Get-Command windeployqt6 -ErrorAction SilentlyContinue) ?? (Get-Command windeployqt)
& $deploy.Source --release --no-translations --no-system-d3d-compiler --no-opengl-sw (Join-Path $app "FlatCAM.exe")
if ($LASTEXITCODE -ne 0) { throw "windeployqt falló" }
# El OpenGL por software (Mesa) de Qt: el canvas dibuja también en máquinas virtuales y escritorios remotos sin GL.
$qtBin = Split-Path -Parent $deploy.Source
$sw = Join-Path $qtBin "opengl32sw.dll"
if (Test-Path $sw) { Copy-Item $sw $app }

# Las DLL de vcpkg (todas menos las de GoogleTest).
$vbin = Join-Path $Vcpkg "installed/$Triplet/bin"
Get-ChildItem $vbin -Filter *.dll | Where-Object { $_.Name -notmatch '^(gtest|gmock)' } | Copy-Item -Destination $app

# La biblioteca de scripts de Tcl (init.tcl) y sus módulos (tcl<mayor>, con msgcat).
$init = Get-ChildItem (Join-Path $Vcpkg "installed/$Triplet") -Recurse -Filter init.tcl |
        Where-Object { $_.Directory.Name -match '^tcl\d+\.\d+$' } | Select-Object -First 1
if ($init) {
    $tcldir = $init.Directory
    Copy-Item -Recurse $tcldir.FullName (Join-Path $app $tcldir.Name)
    $major = "tcl" + ($tcldir.Name -replace '^tcl(\d+)\..*$', '$1')
    $mods = Join-Path $tcldir.Parent.FullName $major
    if (Test-Path $mods) { Copy-Item -Recurse $mods (Join-Path $app $major) }
} else {
    Write-Warning "No encontré la biblioteca de scripts de Tcl: la consola Tcl no va a poder iniciar."
}
Copy-Item (Join-Path $Root "LICENSE"), (Join-Path $Root "NOTICE") $app

$zip = Join-Path $Out "FlatCAM-$version-Windows-x64.zip"
if (Test-Path $zip) { Remove-Item $zip }
Compress-Archive -Path $app -DestinationPath $zip
Write-Host "Listo: $zip"

$iscc = Get-Command iscc -ErrorAction SilentlyContinue
if (-not $iscc) {
    $p = "${env:ProgramFiles(x86)}\Inno Setup 6\ISCC.exe"
    if (Test-Path $p) { $iscc = Get-Item $p }
}
if ($iscc) {
    & ($iscc.Source ?? $iscc.FullName) "/DAppVersion=$version" "/DSourceDir=$((Resolve-Path $app).Path)" `
        "/DOutputDir=$((Resolve-Path $Out).Path)" (Join-Path $Root "tools/packaging/flatcam.iss")
    if ($LASTEXITCODE -ne 0) { throw "Inno Setup falló" }
    Write-Host "Listo: $(Join-Path $Out "FlatCAM-$version-Windows-x64-setup.exe")"
} else {
    Write-Warning "Falta Inno Setup (iscc): solo se armó el zip."
}
