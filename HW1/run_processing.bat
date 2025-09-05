g++ -o data_processor data_processor.cpp
echo Compiling C++ program...
if %errorlevel% equ 0 (
    echo Compilation successful!
    echo Running data processing for Abalone data...
    data_processor.exe abalone_params.txt
    echo Running data processing for Stress data...
    data_processor.exe stress_params.txt
    echo Data processing completed!
) else (
    echo Compilation failed!
)
pause
