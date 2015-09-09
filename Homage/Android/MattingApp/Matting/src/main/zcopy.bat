rem echo off
set DIR=libs\armeabi-v7a
set TARGET=jniLibs\armeabi-v7a

del %TARGET%\libMatting.so
rem copy %DIR%\libMatting.so %TARGET%\libMatting.so
XCOPY libs /S /Y jniLibs

rmdir /S /Q  ..\..\build
pause



