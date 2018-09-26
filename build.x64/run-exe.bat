@setlocal
@set TMPBIN=Release\osm2shp.exe
@rem set TMP3RD=X:\3rdParty.x64\bin
@if NOT EXIST %TMPBIN% goto NOEXE
@rem if NOT EXIST %TMP3RD%\nul goto NO3RD

@REM set PATH=%TMP3RD%;%PATH%

%TMPBIN% %*

@goto END

:NOEXE
@echo NOT EXIST %TMPBIN%! *** FIX ME ***
@goto END

:NO3RD
@echo NOT EXIST %TMP3RD%\nul! *** FIX ME ***
@goto END

:END

