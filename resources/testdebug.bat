@echo off

Rem Change the following
set QUELLDIR=D:\dev\lang\test\datax-weblib\build\debug
set ZIELDIR=D:\dev\sdk\omnis\os4314webview-demo\xcomp

xcopy "%QUELLDIR%\webLib.dll" "%ZIELDIR%" /y
xcopy "%QUELLDIR%\webLib.pdb" "%ZIELDIR%" /y

