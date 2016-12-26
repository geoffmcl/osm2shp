@setlocal
@set TMPPRJ=osm2shp
@echo Building %TMPPRJ% in 64-bits
@set TMPDRV=X:
@set BLDDIR=%CD%
@set TMPLOG=bldlog-1.txt
@set VCVERS=14

@set "SET_BAT=%ProgramFiles(x86)%\Microsoft Visual Studio %VCVERS%.0\VC\vcvarsall.bat"
@if NOT EXIST "%SET_BAT%" goto NOBAT

@set TMPSRC=..
@if NOT EXIST %TMPSRC%\CMakeLists.txt goto NOCM

@set TMPOPTS=
@REM #######################################
@REM *** ADJUST TO SUIT YOUR ENVIRONMENT ***
@set TMPINST=%TMPDRV%\3rdParty.x64
@REM set TMPSHP=F:\FG\18\3rdParty.x64
@set BOOST_ROOT=C:\local\boost_1_62_0

@if NOT EXIST %TMPINST%\nul goto NOXIN
@REM if NOT EXIST %TMPSHP%\nul goto NOSHP
@if NOT EXIST %BOOST_ROOT%\nul goto NOBOOST

@REM set TMPOPTS=%TMPOPTS% -DCMAKE_PREFIX_PATH:PATH=%TMPDRV%\FG\18\3rdParty.x64;F:/Projects/software.x64
@set TMPOPTS=%TMPOPTS% -DCMAKE_INSTALL_PREFIX:PATH=%TMPINST%
@set TMPOPTS=%TMPOPTS% -G "Visual Studio %VCVERS% Win64"
@REM set TMPOPTS=%TMPOPTS% -DCMAKE_PREFIX_PATH:PATH=%TMPDRV%\FG\18\3rdParty.x64;F:/Projects/software.x64
@REM set TMPOPTS=%TMPOPTS% -DCMAKE_PREFIX_PATH:PATH=%TMPSHP%
@REM #######################################

@echo Doing build output to %TMPLOG%
@echo Doing build output to %TMPLOG% > %TMPLOG%
@echo Doing: 'call %SET_BAT% AMD64'
@echo Doing: 'call %SET_BAT% AMD64' >> %TMPLOG%
@call "%SET_BAT%" AMD64 >> %TMPLOG% 2>&1
@if ERRORLEVEL 1 goto ERR0

@REM call setupqt64
@cd %BLDDIR%

@call chkmsvc %TMPPRJ%

@REM Uncomment if multiple build logs required
@REM set TMPLOG=bldlog-%TMPVER%.txt
@REM :RPT
@REM if NOT EXIST %TMPLOG% goto GOTLOG
@REM set /A TMPVER+=1
@REM set TMPLOG=bldlog-%TMPVER%.txt
@REM goto RPT
@REM :GOTLOG

@echo Building source: [%TMPSRC%] %DATE% %TIME% output to %TMPLOG%
@echo Building source: [%TMPSRC%] %DATE% %TIME% >> %TMPLOG%

@echo Doing 'cmake %TMPSRC% %TMPOPTS%'
@echo Doing 'cmake %TMPSRC% %TMPOPTS%' >> %TMPLOG%
@cmake %TMPSRC% %TMPOPTS% >> %TMPLOG% 2>&1
@if ERRORLEVEL 1 goto ERR1

@echo Doing: 'cmake --build . --config Debug'
@echo Doing: 'cmake --build . --config Debug' >> %TMPLOG%
@cmake --build . --config Debug >> %TMPLOG% 2>&1
@if ERRORLEVEL 1 goto ERR2

@REM if these are also required
@REM cmake --build . --config RelWithDebInfo >> %TMPLOG% 2>&1
@REM if ERRORLEVEL 1 goto ERR4

@REM cmake --build . --config MinSizeRel >> %TMPLOG% 2>&1
@REM if ERRORLEVEL 1 goto ERR5

@echo Doing: 'cmake --build . --config Release'
@echo Doing: 'cmake --build . --config Release' >> %TMPLOG%
@cmake --build . --config Release >> %TMPLOG% 2>&1
@if ERRORLEVEL 1 goto ERR3
:DONEREL

@echo Appears a successful build of %TMPPRJ% >> %TMPLOG%
@echo.
@echo Appears a successful build of %TMPPRJ% ...
@echo No install of this project at present...
@goto END

:NOXIN
@echo NOT EXIST %TMPINST%\nul! *** FIX ME ***
@goto ISERR

:NOBOOST
@echo NOT EXIST %BOOST_ROOT%\nul *** FIX ME ***
@goto ISERR

:NOBAT
@echo Can NOT locate MSVC setup batch "%SET_BAT%"! *** FIX ME ***
@goto ISERR

:ERR0
@echo Failed to setup 64-bit MSVC environment! *** FIX ME ***
@goto ISERR

:NOCM
@echo ERROR: Can NOT locate %TMPSRC%\CMakeLists.txt! WHERE IS IT! FIX ME!!!
@goto ISERR

:ERR1
@echo cmake config gen ERROR!
@goto ISERR

:ERR2
@echo cmake build Debug ERROR!
@goto ISERR

:ERR3
@fa4 "mt.exe : general error c101008d:" %TMPLOG% >nul
@if ERRORLEVEL 1 goto ERR33
:ERR34
@echo ERROR: Cmake build Release FAILED!
@goto ISERR
:ERR33
@echo Try again due to this STUPID STUPID STUPID error
@echo Try again due to this STUPID STUPID STUPID error >>%TMPLOG%
cmake --build . --config Release >> %TMPLOG% 2>&1
@if ERRORLEVEL 1 goto ERR34
@goto DONEREL

@REM :ERR4
@echo cmake build RelWithDebInfo ERROR!
@goto ISERR

@REM :ERR5
@echo cmake build MinSizeRel ERROR!
@goto ISERR

:ISERR
@endlocal
@exit /b 1

:END
@endlocal
@exit /b 0

@REM eof
