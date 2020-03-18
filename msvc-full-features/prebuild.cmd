@echo off
SETLOCAL

cd ..\msvc-full-features
echo Done

echo Generating "version.h"...

if NOT "%1"==[] (
set VERSION=%1
)

if "%VERSION%"==[] (
for /F "tokens=*" %%i in ('git describe --tags --always --dirty --match "[0-9]*.*"') do set VERSION=%%i
if "%VERSION%"==[] (
set VERSION=Please install `git` to generate VERSION
)
)

echo VERSION defined as "%VERSION%"
>..\src\version.h echo #define VERSION "%VERSION%"
