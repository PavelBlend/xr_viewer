chcp 1251

@echo off

if not exist "icon\icon.o" (
    magick icon\icon_16.png icon\icon_32.png icon\icon_48.png icon\icon_64.png icon\icon_128.png icon\icon_256.png icon\icon.ico
    windres icon\icon.rc -o icon\icon.o -F pe-x86-64
    echo Compile Icon
)

@echo on

C:\progs\mingw\bin\gcc.exe src\main.c icon\icon.o -o xr_viewer.exe -Wall --std=c11 -m64^
 ^
-IC:\libs\freeglut\freeglut-3.4.0\include ^
-LC:\libs\freeglut\freeglut-3.4.0\build\lib\Release ^
-lfreeglut ^
 ^
-IC:\libs\glew\glew-2.2.0\include ^
-LC:\libs\glew\glew-2.2.0\lib\Release\x64 ^
-lglew32 ^
 ^
-IC:\libs\cglm\cglm-0.9.6\include ^
 ^
-IC:\libs\freeimage\FreeImage3180Win32Win64\FreeImage\Dist\x64 ^
-LC:\libs\freeimage\FreeImage3180Win32Win64\FreeImage\Dist\x64 ^
-lFreeImage ^
 ^
-lopengl32

pause
