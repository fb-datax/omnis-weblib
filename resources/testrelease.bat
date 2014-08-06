@echo off

Rem Change the following values

set QUELLDIR=D:\dev\lang\test\datax-weblib\build\debug
set ZIELDIR=D:\dev\sdk\omnis\os4314wv\xcomp

xcopy "%QUELLDIR%\webLib.dll" "%ZIELDIR%" /y

pause