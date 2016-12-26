@setlocal
@set TMPBIN=Release\osm2shp.exe
@set TMP3RD=X:\3rdParty.x64\bin
@if NOT EXIST %TMPBIN% goto NOEXE
@if NOT EXIST %TMP3RD%\nul goto NO3RD

@set PATH=%TMP3RD%;%PATH%

%TMPBIN% %*

@goto END

:NOEXE
@echo NOT EXIST %TMPBIN%! *** FIX ME ***
@goto END

:NO3RD
@echo NOT EXIST %TMP3RD%\nul! *** FIX ME ***
@goto END

:END

