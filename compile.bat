@echo off
set PATH=C:\Qt\6.11.1\mingw_64\bin;C:\Qt\Tools\mingw1310_64\bin;%PATH%
cd src
qmake.exe
mingw32-make.exe
cd ..
pause
@echo on
