param(
    [string]$QtDir = "C:\Qt\6.11.0\mingw_64\bin",
    [string]$Generator = "MinGW Makefiles",
    [string]$BuildType = "Release"
)

function Abort($msg) { Write-Error $msg; exit 1 }

if (-not (Get-Command cmake -ErrorAction SilentlyContinue)) {
    Abort "CMake n'est pas disponible dans le PATH. Installez CMake ou ajoutez-le au PATH."
}

if (-not (Test-Path $QtDir)) {
    $ok = Read-Host "Le chemin Qt spécifié '$QtDir' n'existe pas. Entrez le chemin Qt (ou tapez 'cancel')"
    if ($ok -eq 'cancel') { Abort "Annulation par l'utilisateur." }
    $QtDir = $ok
    if (-not (Test-Path $QtDir)) { Abort "Chemin Qt invalide : $QtDir" }
}

$CMakePrefix = Join-Path -Path $QtDir -ChildPath "lib\cmake"
if (-not (Test-Path $CMakePrefix)) { Abort "Chemin attendu introuvable : $CMakePrefix" }

$script:here = Split-Path -Path $MyInvocation.MyCommand.Definition -Parent
Set-Location -Path $script:here

$buildDir = Join-Path -Path $script:here -ChildPath "build"
if (-not (Test-Path $buildDir)) { New-Item -ItemType Directory -Path $buildDir | Out-Null }
Set-Location -Path $buildDir

Write-Host "Configuring with CMake (Qt prefix: $CMakePrefix) ..."
$cmakeArgs = @(
    '-G', $Generator,
    "-DCMAKE_PREFIX_PATH=$CMakePrefix",
    '..'
)
$logFile = Join-Path -Path $buildDir -ChildPath "build_log.txt"
if (Test-Path $logFile) { Remove-Item $logFile -ErrorAction SilentlyContinue }
$qtGpp = Get-ChildItem -Path $QtDir -Filter g++.exe -Recurse -ErrorAction SilentlyContinue | Select-Object -First 1
if ($qtGpp) {
    $qtBin = Split-Path $qtGpp.FullName -Parent
    Write-Host "Detected Qt-provided g++ at: $($qtGpp.FullName)"
    Write-Host "Adding $qtBin to PATH for this session."
    $env:Path = "$qtBin;" + $env:Path
} else {
    Write-Host "Warning: no g++.exe found under $QtDir. Ensure a compatible MinGW is installed or use the MinGW bundled with Qt."
}

$cmakeArgs | Out-File -FilePath $logFile -Append -Encoding utf8
Write-Host "Running: cmake $($cmakeArgs -join ' ')"
$procOut = & cmake @cmakeArgs 2>&1 | Tee-Object -FilePath $logFile
if ($LASTEXITCODE -ne 0) { Write-Host "CMake configure failed. Showing tail of ${logFile}:"; Get-Content $logFile -Tail 200; Abort "CMake configuration failed." }

Write-Host "Building (configuration: $BuildType) ..."
$buildArgs = @('--build', '.', '--config', $BuildType)
Write-Host "Running: cmake $($buildArgs -join ' ')"
$procOut = & cmake @buildArgs 2>&1 | Tee-Object -FilePath $logFile -Append
if ($LASTEXITCODE -ne 0) { Write-Host "Build failed. Showing tail of ${logFile}:"; Get-Content $logFile -Tail 400; Abort "Build failed." }

# Add Qt bin to PATH for runtime
$qtBin = Join-Path -Path $QtDir -ChildPath "bin"
$env:Path = "$qtBin;" + $env:Path

# Try to find executable
$exe = Get-ChildItem -Path . -Filter "alterdune_qt*" -Recurse -ErrorAction SilentlyContinue | Select-Object -First 1
if ($exe) {
    Write-Host "Exécutable trouvé : $($exe.FullName)"
    Write-Host "Lancement de l'application..."
    & $exe.FullName
} else {
    Write-Host "Build terminé, mais aucun exécutable 'alterdune_qt' trouvé dans le build dir. Vérifiez le générateur et les cibles CMake." 
}
