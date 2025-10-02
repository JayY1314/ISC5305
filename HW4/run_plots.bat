@echo off
echo Building visualization program...
g++ -std=c++17 -Wall -Wextra -o main_visualization main_visualization.cpp

if %ERRORLEVEL% neq 0 (
    echo Compilation failed!
    pause
    exit /b 1
)

echo Running visualization program...
main_visualization.exe

if %ERRORLEVEL% neq 0 (
    echo Program execution failed!
    pause
    exit /b 1
)

echo.
echo Generating plots with Python...
python create_plots.py

if %ERRORLEVEL% neq 0 (
    echo Python script failed! Make sure you have matplotlib and pandas installed.
    echo Install with: pip install matplotlib pandas
    pause
    exit /b 1
)

echo.
echo All done! Check the generated files:
echo - newton_plot.csv
echo - secant_plot.csv  
echo - root_convergence_plot.png
echo - residual_convergence_plot.png
pause
