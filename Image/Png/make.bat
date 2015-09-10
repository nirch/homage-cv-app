setlocal
echo off
set CONFIG=Release
set TARGET=""
rem prompt %CONFIG%

if "%1" NEQ "" set CONFIG=%1

if "%1"==""  goto SMELSE
if "%2"=="" goto SMELSE

prompt %2
prompt  %CONFIG%


evenv Png.sln /MAKE "%2 - Win32 %CONFIG%"
goto EXIT
:SMELSE

echo on
devenv Png.sln /Build  %CONFIG%  /project PngCodec
echo off




:EXIT
rem pause