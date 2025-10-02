@echo off
echo Compiling verify_errors program...
g++ -std=c++17 -Wall -Wextra -o verify_errors verify_errors.cpp

if %ERRORLEVEL% neq 0 (
    echo Compilation failed!
    pause
    exit /b 1
)

echo Running verify_errors program...
verify_errors.exe

if %ERRORLEVEL% neq 0 (
    echo Program execution failed!
    pause
    exit /b 1
)

echo.
echo Verify errors table generated!
echo Check verify_errors.txt for the complete table.
pause
