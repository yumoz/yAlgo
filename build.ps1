# One-click cmake configure + build for a chosen platform + configuration
# Usage:
#   .\build.ps1                          # windows + debug
#   .\build.ps1 -Config release         # windows + release
#   .\build.ps1 -Platform linux         # linux + debug
#   .\build.ps1 -Platform kylin -Arch x86 -Config release
#   .\build.ps1 -Config debug -Clean
#   .\build.ps1 -Jobs 8                 # parallel build, 8 threads
#   .\build.ps1 -Install                # cmake --install after build
#   .\build.ps1 -NoTest                 # skip CTest
#   .\build.ps1 -Help                   # show usage

[CmdletBinding()]
param(
    [string]$Platform = "windows",
    [string]$Arch     = "arm",
    [string]$Config   = "debug",
    [int]$Jobs        = 0,
    [switch]$Clean,
    [switch]$Install,
    [switch]$NoTest,
    [switch]$Help
)

$ErrorActionPreference = "Stop"

if ($Help) {
    Write-Host @"

build.ps1 - one-click cmake configure + build

Parameters:
  -Platform <name>   Target platform  (windows* | linux | macos | kylin)
  -Arch     <name>   CPU arch, kylin only (arm* | x86)
  -Config   <name>   Build type       (debug* | release)
  -Jobs     <int>    Parallel build threads (0 = cmake default)
  -Clean             Remove build/<preset> before building
  -Install           Run cmake --install after build
  -NoTest            Skip automatic CTest at the end
  -Help              Show this help and exit

Preset mapping (platform + arch + config):
  windows            -> windows-debug / windows-release
  linux              -> linux-debug   / linux-release
  macos              -> macos-debug   / macos-release
  kylin arm (def)    -> kylin-arm-debug / kylin-arm-release
  kylin x86          -> kylin-x86-debug / kylin-x86-release

Examples:
  .\build.ps1
  .\build.ps1 -Config release
  .\build.ps1 -Platform linux -Config release -Jobs 8
  .\build.ps1 -Platform kylin -Arch x86 -Config release
  .\build.ps1 -Config debug -Clean -Install -NoTest

Note: CTest runs automatically at the end ONLY when the target
platform matches the host OS (windows/linux/macos), unless -NoTest.

"@
    exit 0
}

$plat = $Platform.Trim().ToLower()
$arch = $Arch.Trim().ToLower()
$cfg  = $Config.Trim().ToLower()

# resolve preset name from platform + arch + config
switch ($plat) {
    { $_ -in @("windows", "win", "w") } {
        if     ($cfg -in @("debug", "d")) { $preset = "windows-debug" }
        elseif ($cfg -in @("release", "r")) { $preset = "windows-release" }
        else { Write-Error "Unknown config '$Config'. Use: debug | release"; exit 1 }
        break
    }
    { $_ -in @("linux", "l") } {
        if     ($cfg -in @("debug", "d")) { $preset = "linux-debug" }
        elseif ($cfg -in @("release", "r")) { $preset = "linux-release" }
        else { Write-Error "Unknown config '$Config'. Use: debug | release"; exit 1 }
        break
    }
    { $_ -in @("macos", "mac", "m") } {
        if     ($cfg -in @("debug", "d")) { $preset = "macos-debug" }
        elseif ($cfg -in @("release", "r")) { $preset = "macos-release" }
        else { Write-Error "Unknown config '$Config'. Use: debug | release"; exit 1 }
        break
    }
    { $_ -in @("kylin", "k") } {
        if ($arch -in @("x86", "x86_64", "amd64")) {
            if     ($cfg -in @("debug", "d")) { $preset = "kylin-x86-debug" }
            elseif ($cfg -in @("release", "r")) { $preset = "kylin-x86-release" }
            else { Write-Error "Unknown config '$Config'. Use: debug | release"; exit 1 }
        } else {
            if     ($cfg -in @("debug", "d")) { $preset = "kylin-arm-debug" }
            elseif ($cfg -in @("release", "r")) { $preset = "kylin-arm-release" }
            else { Write-Error "Unknown config '$Config'. Use: debug | release"; exit 1 }
        }
        break
    }
    default {
        Write-Error "Unknown platform '$Platform'. Use: windows | linux | macos | kylin"
        exit 1
    }
}

if ($Jobs -lt 0) {
    Write-Error "-Jobs must be >= 0"
    exit 1
}

$ProjectRoot = Split-Path -Parent $MyInvocation.MyCommand.Path
$BuildDir    = Join-Path $ProjectRoot "build\$preset"

Write-Host "==> Project root: $ProjectRoot" -ForegroundColor Cyan
Write-Host "==> Platform: $plat  Arch: $arch  Config: $cfg  Jobs: $Jobs" -ForegroundColor Cyan
Write-Host "==> Preset: $preset" -ForegroundColor Cyan

$cmakeCmd = Get-Command cmake -ErrorAction SilentlyContinue
if ($null -eq $cmakeCmd) {
    Write-Error "cmake not found. Please add it to PATH."
    exit 1
}

if ($Clean) {
    if (Test-Path $BuildDir) {
        Write-Host "==> Cleaning $BuildDir" -ForegroundColor Yellow
        Remove-Item $BuildDir -Recurse -Force
    }
}

Write-Host "==> Configure ($preset)" -ForegroundColor Cyan
cmake --preset $preset
if ($LASTEXITCODE -ne 0) { exit $LASTEXITCODE }

$buildArgs = "--preset", $preset
if ($Jobs -gt 0) { $buildArgs += "-j", "$Jobs" }

Write-Host "==> Build ($preset)" -ForegroundColor Cyan
cmake --build @buildArgs
if ($LASTEXITCODE -ne 0) { exit $LASTEXITCODE }

Write-Host "==> Build done ($preset)" -ForegroundColor Green

if ($Install) {
    Write-Host "==> Install ($preset)" -ForegroundColor Cyan
    cmake --install $BuildDir
    if ($LASTEXITCODE -ne 0) { exit $LASTEXITCODE }
}

if ($NoTest) {
    Write-Host "==> Skip CTest (-NoTest)" -ForegroundColor Yellow
    exit 0
}

# Auto-run CTest only when target platform matches host OS
$hostOs = "unknown"
if ($IsWindows) { $hostOs = "windows" }
elseif ($IsLinux) { $hostOs = "linux" }
elseif ($IsMacOS) { $hostOs = "macos" }

$runTest = $false
if ($plat -eq "windows" -and $hostOs -eq "windows") { $runTest = $true }
elseif ($plat -eq "linux" -and $hostOs -eq "linux") { $runTest = $true }
elseif ($plat -eq "macos" -and $hostOs -eq "macos") { $runTest = $true }
elseif ($plat -eq "kylin" -and $hostOs -eq "linux") { $runTest = $true }

if ($runTest) {
    Write-Host "==> Run tests ($preset)" -ForegroundColor Cyan
    ctest --build-config $cfg --test-dir $BuildDir
    if ($LASTEXITCODE -ne 0) { exit $LASTEXITCODE }
} else {
    Write-Host "==> Skip CTest (target '$plat' != host '$hostOs')" -ForegroundColor Yellow
}
