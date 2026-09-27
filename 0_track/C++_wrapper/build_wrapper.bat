@echo off
rem ============================================================================================
rem Build script for motive_wrapper.dll (x64, MSVC)
rem Usage:  build_wrapper.bat [path-to-Motive-install]
rem Default Motive install: C:\Program Files\OptiTrack\Motive
rem Output: ..\bin\motive_wrapper.dll  (+ MotiveAPI.dll copied alongside)
rem ============================================================================================

setlocal EnableExtensions

set "MOTIVE_DIR=%~1"
if "%MOTIVE_DIR%"=="" set "MOTIVE_DIR=C:\Program Files\OptiTrack\Motive"

set "INC_DIR=%MOTIVE_DIR%\inc"
set "LIB_DIR=%MOTIVE_DIR%\lib"
set "OUT_DIR=%~dp0..\bin"

if not exist "%INC_DIR%\MotiveAPI.h" (
    echo [ERROR] MotiveAPI.h not found in %INC_DIR%
    exit /b 1
)
if not exist "%LIB_DIR%\MotiveAPI.lib" (
    echo [ERROR] MotiveAPI.lib not found in %LIB_DIR%
    exit /b 1
)

rem --- Locate vcvars64.bat -------------------------------------------------------------
set "VCVARS="
if exist "C:\Program Files\Microsoft Visual Studio\2022\Community\VC\Auxiliary\Build\vcvars64.bat" set "VCVARS=C:\Program Files\Microsoft Visual Studio\2022\Community\VC\Auxiliary\Build\vcvars64.bat"
if exist "C:\Program Files\Microsoft Visual Studio\2022\Professional\VC\Auxiliary\Build\vcvars64.bat" set "VCVARS=C:\Program Files\Microsoft Visual Studio\2022\Professional\VC\Auxiliary\Build\vcvars64.bat"
if exist "C:\Program Files\Microsoft Visual Studio\2022\Enterprise\VC\Auxiliary\Build\vcvars64.bat" set "VCVARS=C:\Program Files\Microsoft Visual Studio\2022\Enterprise\VC\Auxiliary\Build\vcvars64.bat"
if exist "C:\Program Files (x86)\Microsoft Visual Studio\2019\Community\VC\Auxiliary\Build\vcvars64.bat" set "VCVARS=C:\Program Files (x86)\Microsoft Visual Studio\2019\Community\VC\Auxiliary\Build\vcvars64.bat"
if exist "C:\Program Files (x86)\Microsoft Visual Studio\2019\Professional\VC\Auxiliary\Build\vcvars64.bat" set "VCVARS=C:\Program Files (x86)\Microsoft Visual Studio\2019\Professional\VC\Auxiliary\Build\vcvars64.bat"
if exist "C:\Program Files (x86)\Microsoft Visual Studio\18\BuildTools\VC\Auxiliary\Build\vcvars64.bat" set "VCVARS=C:\Program Files (x86)\Microsoft Visual Studio\18\BuildTools\VC\Auxiliary\Build\vcvars64.bat"

if not defined VCVARS (
    echo [ERROR] Could not locate vcvars64.bat. Install Visual Studio Build Tools with the x64 C++ workload.
    exit /b 1
)
echo [INFO] Using toolchain: %VCVARS%

rem --- Compile & link ---------------------------------------------------------------------
if not exist "%OUT_DIR%" mkdir "%OUT_DIR%"
if not exist "%OUT_DIR%\obj" mkdir "%OUT_DIR%\obj"

call "%VCVARS%" x64 >nul

cl.exe /nologo /O2 /W3 /MD /EHsc /std:c++17 /DMOTIVE_API_IMPORTS /DMOTIVE_WRAPPER_EXPORTS /I"%INC_DIR%" /Fe"%OUT_DIR%\motive_wrapper.dll" /Fo"%OUT_DIR%\obj\\" "%~dp0motive_wrapper.cpp" /link /DLL /SUBSYSTEM:WINDOWS "%LIB_DIR%\MotiveAPI.lib"

if errorlevel 1 (
    echo [ERROR] Compilation failed.
    exit /b 1
)

rem NOTE: MotiveAPI.dll is intentionally NOT copied. It must stay in the Motive
rem install directory because it resolves Qt resources (qt.conf) relative to
rem its own location. The Python client adds that directory to the DLL path.
echo [INFO] Built: %OUT_DIR%\motive_wrapper.dll
endlocal
