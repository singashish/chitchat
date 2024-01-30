@echo off

set BUILD_DIR=build

rem Check if the build directory exists, and if so, delete it
if exist %BUILD_DIR% (
    rmdir /s /q %BUILD_DIR%
)

rem Create the build directory
echo.
mkdir %BUILD_DIR%
cd %BUILD_DIR%
cmake -G "MinGW Makefiles" ..
echo.
cmake --build .
cd ..