@echo off
chcp 65001 >nul 2>&1
setlocal enabledelayedexpansion

set ROOT=%~dp0

rem ---- parse args (flags can appear anywhere) ----
set PLAT=windows
set ARCH=arm
set CFG=debug
set JOBS=0
set DO_CLEAN=0
set DO_INSTALL=0
set DO_NOTEST=0
set DO_HELP=0

:parse
if "%1"=="" goto :parse_done
if /i "%1"=="-Help" ( set DO_HELP=1 & shift & goto :parse )
if /i "%1"=="-h"    ( set DO_HELP=1 & shift & goto :parse )
if /i "%1"=="/?"    ( set DO_HELP=1 & shift & goto :parse )
if /i "%1"=="-Clean"   ( set DO_CLEAN=1 & shift & goto :parse )
if /i "%1"=="-Install" ( set DO_INSTALL=1 & shift & goto :parse )
if /i "%1"=="-NoTest"  ( set DO_NOTEST=1 & shift & goto :parse )
if /i "%1"=="-Platform" ( set PLAT=%2 & shift & shift & goto :parse )
if /i "%1"=="-Arch"     ( set ARCH=%2 & shift & shift & goto :parse )
if /i "%1"=="-Config"   ( set CFG=%2 & shift & shift & goto :parse )
if /i "%1"=="-Jobs"     ( set JOBS=%2 & shift & shift & goto :parse )
rem positional fallback: 1st unnamed = platform, 2nd = arch, 3rd = config
if not defined SEEN_POS1 ( set PLAT=%1 & set SEEN_POS1=1 & shift & goto :parse )
if not defined SEEN_POS2 ( set ARCH=%1 & set SEEN_POS2=1 & shift & goto :parse )
if not defined SEEN_POS3 ( set CFG=%1 & set SEEN_POS3=1 & shift & goto :parse )
shift
goto :parse
:parse_done

if "%DO_HELP%"=="1" goto :help

rem defaults for positional
if "%PLAT%"=="" set PLAT=windows
if "%ARCH%"=="" set ARCH=arm
if "%CFG%"=="" set CFG=debug

set PRESET=
if /i "%PLAT%"=="windows" goto :win
if /i "%PLAT%"=="win" goto :win
if /i "%PLAT%"=="w" goto :win
if /i "%PLAT%"=="linux" goto :lin
if /i "%PLAT%"=="l" goto :lin
if /i "%PLAT%"=="macos" goto :mac
if /i "%PLAT%"=="mac" goto :mac
if /i "%PLAT%"=="m" goto :mac
if /i "%PLAT%"=="kylin" goto :kyl
if /i "%PLAT%"=="k" goto :kyl
echo Unknown platform '%PLAT%'. Use: windows ^| linux ^| macos ^| kylin
exit /b 1

:win
if /i "%CFG%"=="release" set PRESET=windows-release
if /i "%CFG%"=="r" set PRESET=windows-release
if /i "%CFG%"=="debug" set PRESET=windows-debug
if /i "%CFG%"=="d" set PRESET=windows-debug
goto :check

:lin
if /i "%CFG%"=="release" set PRESET=linux-release
if /i "%CFG%"=="r" set PRESET=linux-release
if /i "%CFG%"=="debug" set PRESET=linux-debug
if /i "%CFG%"=="d" set PRESET=linux-debug
goto :check

:mac
if /i "%CFG%"=="release" set PRESET=macos-release
if /i "%CFG%"=="r" set PRESET=macos-release
if /i "%CFG%"=="debug" set PRESET=macos-debug
if /i "%CFG%"=="d" set PRESET=macos-debug
goto :check

:kyl
if /i "%ARCH%"=="x86" goto :kylx86
if /i "%ARCH%"=="x86_64" goto :kylx86
if /i "%ARCH%"=="amd64" goto :kylx86
if /i "%CFG%"=="release" set PRESET=kylin-arm-release
if /i "%CFG%"=="r" set PRESET=kylin-arm-release
if /i "%CFG%"=="debug" set PRESET=kylin-arm-debug
if /i "%CFG%"=="d" set PRESET=kylin-arm-debug
goto :check
:kylx86
if /i "%CFG%"=="release" set PRESET=kylin-x86-release
if /i "%CFG%"=="r" set PRESET=kylin-x86-release
if /i "%CFG%"=="debug" set PRESET=kylin-x86-debug
if /i "%CFG%"=="d" set PRESET=kylin-x86-debug
goto :check

:check
if "%PRESET%"=="" (
    echo Unknown config '%CFG%'. Use: debug ^| release
    exit /b 1
)

set BUILDDIR=%ROOT%build\%PRESET%

echo ==^> Project root: %ROOT%
echo ==^> Platform: %PLAT%  Arch: %ARCH%  Config: %CFG%  Jobs: %JOBS%
echo ==^> Preset: %PRESET%

where cmake >nul 2>nul
if errorlevel 1 (
    echo cmake not found. Please add it to PATH.
    exit /b 1
)

if "%DO_CLEAN%"=="1" (
    if exist "%BUILDDIR%" (
        echo ==^> Cleaning %BUILDDIR%
        rmdir /s /q "%BUILDDIR%"
    )
)

echo ==^> Configure (%PRESET%)
cmake --preset %PRESET%
if errorlevel 1 exit /b %errorlevel%

echo ==^> Build (%PRESET%)
if "%JOBS%"=="0" (
    cmake --build --preset %PRESET%
) else (
    cmake --build --preset %PRESET% -j %JOBS%
)
if errorlevel 1 exit /b %errorlevel%

echo ==^> Build done (%PRESET%)

if "%DO_INSTALL%"=="1" (
    echo ==^> Install (%PRESET%)
    cmake --install "%BUILDDIR%"
    if errorlevel 1 exit /b %errorlevel%
)

if "%DO_NOTEST%"=="1" (
    echo ==^> Skip CTest (-NoTest)
    endlocal
    goto :eof
)

rem Auto CTest only when target platform matches host OS
set RUNTEST=0
if /i "%PLAT%"=="windows" if defined OS (
    if /i "%OS%"=="Windows_NT" set RUNTEST=1
)
if /i "%PLAT%"=="linux" if not defined OS set RUNTEST=1
if /i "%PLAT%"=="macos" if not defined OS set RUNTEST=1
if /i "%PLAT%"=="kylin" if not defined OS set RUNTEST=1

if "%RUNTEST%"=="1" (
    echo ==^> Run tests (%PRESET%)
    ctest --build-config %CFG% --test-dir "%BUILDDIR%"
    if errorlevel 1 exit /b %errorlevel%
) else (
    echo ==^> Skip CTest (target '%PLAT%' != host)
)
endlocal
goto :eof

:help
echo.
echo build.bat - one-click cmake configure + build
echo.
echo Usage (flags anywhere; positional fallback: Platform Arch Config):
echo   build.bat [Platform] [Arch] [Config] [-Clean] [-NoTest] [-Install] [-Jobs N] [-Help]
echo   build.bat -Platform linux -Config release -Jobs 8
echo.
echo Parameters:
echo   -Platform ^<name^>   windows* ^| linux ^| macos ^| kylin
echo   -Arch     ^<name^>   arm* ^| x86   (kylin only)
echo   -Config   ^<name^>   debug* ^| release
echo   -Jobs     ^<int^>    parallel build threads (0 = cmake default)
echo   -Clean             remove build^<preset^> before building
echo   -Install           run cmake --install after build
echo   -NoTest            skip automatic CTest
echo   -Help              show this help
echo.
echo Preset mapping:
echo   windows            -^> windows-debug  / windows-release
echo   linux              -^> linux-debug    / linux-release
echo   macos              -^> macos-debug    / macos-release
echo   kylin arm (def)    -^> kylin-arm-debug  / kylin-arm-release
echo   kylin x86          -^> kylin-x86-debug  / kylin-x86-release
echo.
echo Examples:
echo   build.bat
echo   build.bat windows "" release -Jobs 8
echo   build.bat linux "" release -Install
echo   build.bat kylin x86 release -NoTest
echo   build.bat debug -Clean -Install
echo.
echo Note: CTest runs automatically at the end ONLY when the target
echo       platform matches the host OS, unless -NoTest is given.
endlocal
goto :eof
