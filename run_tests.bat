@echo off
echo ========================================
echo Running Google Tests...
build\Release\solver_tests.exe
if %errorlevel% neq 0 (
    echo Tests FAILED!
    pause
    exit /b 1
)

echo.
echo ========================================
echo All tests passed!
pause