@setlocal
@set TMPEXE=osm2shp.exe
@set TMPSRC=Release\%TMPEXE%
@if NOT EXIST %TMPSRC% goto NOEXE
@call dirmin %TMPSRC%
@set TMPDIR=C:\MDOS
@if NOT EXIST %TMPDIR%\nul goto NODIR
@set TMPDST=%TMPDIR%\%TMPEXE%
@if NOT EXIST %TMPDST% goto DOCOPY
@call dirmin %TMPDST%
@fc4 -v0 -q -b %TMPSRC% %TMPDST%
@if ERRORLEVEL 1 goto DOCOPY
@echo. 
@echo Files appear the SAME! Nothing done...
@echo.
@goto END

:DOCOPY
@echo Will copy %TMPSRC% to %TMPDST%
@pause
copy %TMPSRC% %TMPDST%
@goto END

:NOEXE
@echo Can NOT locate %TMPSRC%!
@goto END

:NODIR
@echo Can NOT locate %TMPDIR%!
@goto END

:END

@REM eof
