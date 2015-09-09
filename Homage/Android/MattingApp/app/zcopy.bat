set DIR=D:\_work\Gp_Vim\Homage\Android\MattingApp\Matting\build\outputs\aar
set TARGET=libs

del %TARGET%\Matting.aar
copy %DIR%\Matting-debug.aar %TARGET%\Matting.aar

rmdir /S /Q  build
pause



