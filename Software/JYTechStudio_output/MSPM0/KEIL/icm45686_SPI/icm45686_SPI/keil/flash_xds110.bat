@echo off
setlocal

set "UNIFLASH_ROOT=%UNIFLASH_ROOT%"
if not defined UNIFLASH_ROOT set "UNIFLASH_ROOT=C:\ti\uniflash_9.2.0"

set "DSLITE=%UNIFLASH_ROOT%\dslite.bat"
set "CONFIG=%~dp0targetConfigs\MSPM0G3507.ccxml"
set "AXF=%~dp0Objects\empty_LP_MSPM0G3507_nortos_keil.axf"

if not exist "%DSLITE%" (
    echo [XDS110] UniFlash not found: "%DSLITE%"
    exit /b 1
)

if not exist "%CONFIG%" (
    echo [XDS110] Target configuration not found: "%CONFIG%"
    exit /b 1
)

if not exist "%AXF%" (
    echo [XDS110] Build output not found: "%AXF%"
    exit /b 1
)

call "%DSLITE%" --config="%CONFIG%" --flash --verify --reset 1 --run --verbose "%AXF%"
exit /b %ERRORLEVEL%
