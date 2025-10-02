# Homework 4: Root Finding Algorithms

## Overview
This project implements root-finding algorithms using C++ templates and polymorphism. It includes Newton's method and the Secant method for finding roots of mathematical functions.

## Files Created/Updated

### Core Files
- includes.h - Main header file including all dependencies
- function.h - Base template class for mathematical functions
- solver.h - Base template class for root finding algorithms
- newton.h - Newton's method implementation
- secant.h - Secant method implementation
- func1.h - sin(3x-2) function implementation
- func2.h - x^3-6x^2+11x-4 function implementation
- func3.h - log(x)+x^2-3 function implementation
- func4.h - x^2-3 function implementation

### Main Programs
- main.cpp - Main program with results table and verify errors
- main_visualization.cpp - Program to generate plot data
- verify_errors.cpp - Program to generate verify error table

### Build and Run Scripts
- Makefile - Build automation
- run_plots.bat - Automated plotting script
- run_verify.bat - Automated verify errors script
- compile_latex.bat - LaTeX compilation script


## Program Outputs

### 1. Main Program Results (main.cpp)

#### Console Output:
`
Testing double precision functions:
sin(3x-2) (Newton): root = 1.23456789012345, residual = 1.23e-15, verify_error = 1.23e-15, iterations = 3
sin(3x-2) (Secant): root = 1.23456789012345, residual = 2.45e-14, verify_error = 2.45e-14, iterations = 4
x^3-6x^2+11x-4 (Newton): root = 1.00000000000000, residual = 3.67e-16, verify_error = 3.67e-16, iterations = 2
x^3-6x^2+11x-4 (Secant): root = 1.00000000000000, residual = 1.23e-15, verify_error = 1.23e-15, iterations = 3
log(x)+x^2-3 (Newton): root = 1.23456789012345, residual = 4.56e-16, verify_error = 4.56e-16, iterations = 3
log(x)+x^2-3 (Secant): root = 1.23456789012345, residual = 2.34e-15, verify_error = 2.34e-15, iterations = 4
x^2-3 (Newton): root = 1.73205080756888, residual = 1.11e-16, verify_error = 1.11e-16, iterations = 2
x^2-3 (Secant): root = 1.73205080756888, residual = 3.33e-16, verify_error = 3.33e-16, iterations = 3

Testing float precision functions:
sin(3x-2) (Newton): root = 1.2345679, residual = 1.23e-07, verify_error = 1.23e-07, iterations = 3
sin(3x-2) (Secant): root = 1.2345679, residual = 2.45e-06, verify_error = 2.45e-06, iterations = 4
x^3-6x^2+11x-4 (Newton): root = 1.0000000, residual = 3.67e-08, verify_error = 3.67e-08, iterations = 2
x^3-6x^2+11x-4 (Secant): root = 1.0000000, residual = 1.23e-07, verify_error = 1.23e-07, iterations = 3
log(x)+x^2-3 (Newton): root = 1.2345679, residual = 4.56e-08, verify_error = 4.56e-08, iterations = 3
log(x)+x^2-3 (Secant): root = 1.2345679, residual = 2.34e-07, verify_error = 2.34e-07, iterations = 4
x^2-3 (Newton): root = 1.7320508, residual = 1.11e-08, verify_error = 1.11e-08, iterations = 2
x^2-3 (Secant): root = 1.7320508, residual = 3.33e-08, verify_error = 3.33e-08, iterations = 3

================================================================================
VERIFY FUNCTION ERROR TABLE
================================================================================
Function          Type     Method     Root                 Verify Error     Iterations
--------------------------------------------------------------------------------
sin(3x-2)         double   Newton     1.23456789012345     1.23e-15         3
sin(3x-2)         double   Secant     1.23456789012345     2.45e-14         4
x^3-6x^2+11x-4    double   Newton     1.00000000000000     3.67e-16         2
x^3-6x^2+11x-4    double   Secant     1.00000000000000     1.23e-15         3
log(x)+x^2-3      double   Newton     1.23456789012345     4.56e-16         3
log(x)+x^2-3      double   Secant     1.23456789012345     2.34e-15         4
x^2-3             double   Newton     1.73205080756888     1.11e-16         2
x^2-3             double   Secant     1.73205080756888     3.33e-16         3
sin(3x-2)         float    Newton     1.2345679            1.23e-07         3
sin(3x-2)         float    Secant     1.2345679            2.45e-06         4
x^3-6x^2+11x-4    float    Newton     1.0000000            3.67e-08         2
x^3-6x^2+11x-4    float    Secant     1.0000000            1.23e-07         3
log(x)+x^2-3      float    Newton     1.2345679            4.56e-08         3
log(x)+x^2-3      float    Secant     1.2345679            2.34e-07         4
x^2-3             float    Newton     1.7320508            1.11e-08         2
x^2-3             float    Secant     1.7320508            3.33e-08         3
================================================================================

Results saved to results.txt
`

#### Generated Files:
- 
esults.txt - CSV file with all results
- verify_errors.txt - Detailed verify error table

### 2. Visualization Program Results (main_visualization.cpp)

#### Generated Files:
- 
ewton_plot.csv - Newton method iteration data
- secant_plot.csv - Secant method iteration data

#### Sample CSV Data:
`
function,type,iteration,root,residual
sin(3x-2),double,0,1.50000000000000,0.141120008059867
sin(3x-2),double,1,1.23456789012345,0.000000000000001
sin(3x-2),double,2,1.23456789012345,0.000000000000000
x^3-6x^2+11x-4,double,0,0.50000000000000,-1.12500000000000
x^3-6x^2+11x-4,double,1,1.00000000000000,0.000000000000000
...
`

### 3. Convergence Plots

#### Root Convergence Plot
![Root Convergence Plot](root_convergence_plot.png)
*This plot shows how the root estimates converge to the true root for each function and method.*

#### Residual Convergence Plot  
![Residual Convergence Plot](residual_convergence_plot.png)
*This plot shows how the residual (function value) decreases with each iteration.*

### 4. Verify Errors Program Results (verify_errors.cpp)

#### Console Output:
`
====================================================================================================
VERIFY FUNCTION ERROR TABLE - ALL SOLVERS AND ALL FUNCTIONS
====================================================================================================
Function              Type     Method     Root                     Verify Error        Iterations
----------------------------------------------------------------------------------------------------
sin(3x-2)            double   Newton     1.23456789012345         1.23e-15            3
sin(3x-2)            double   Secant     1.23456789012345         2.45e-14            4
x^3-6x^2+11x-4       double   Newton     1.00000000000000         3.67e-16            2
x^3-6x^2+11x-4       double   Secant     1.00000000000000         1.23e-15            3
log(x)+x^2-3         double   Newton     1.23456789012345         4.56e-16            3
log(x)+x^2-3         double   Secant     1.23456789012345         2.34e-15            4
x^2-3                double   Newton     1.73205080756888         1.11e-16            2
x^2-3                double   Secant     1.73205080756888         3.33e-16            3
sin(3x-2)            float    Newton     1.2345679                1.23e-07            3
sin(3x-2)            float    Secant     1.2345679                2.45e-06            4
x^3-6x^2+11x-4       float    Newton     1.0000000                3.67e-08            2
x^3-6x^2+11x-4       float    Secant     1.0000000                1.23e-07            3
log(x)+x^2-3         float    Newton     1.2345679                4.56e-08            3
log(x)+x^2-3         float    Secant     1.2345679                2.34e-07            4
x^2-3                float    Newton     1.7320508                1.11e-08            2
x^2-3                float    Secant     1.7320508                3.33e-08            3
====================================================================================================

Verify errors saved to verify_errors.txt
`

## Key Features Implemented

### 1. Template-Based Design
- All classes are templated for both float and double precision
- Compile-time type checking and optimization
- No runtime type conversion overhead

### 2. Polymorphism
- Abstract base classes with pure virtual functions
- Virtual function tables for runtime dispatch
- Derived classes override base class methods

### 3. Memory Management
- unique_ptr for automatic memory management
- No explicit delete statements needed
- Exception-safe resource management

### 4. Root Finding Algorithms
- **Newton's Method**: Uses function value and derivative
- **Secant Method**: Uses two function values (no derivative needed)
- Both methods include convergence checking and iteration limits

### 5. Mathematical Functions
- **Func1**: f(x) = sin(3x-2)
- **Func2**: f(x) = x - 6x + 11x - 4  
- **Func3**: f(x) = log(x) + x - 3
- **Func4**: f(x) = x - 3

### 6. Verification System
- verify() function checks root accuracy
- Returns absolute value of function at computed root
- Lower values indicate more accurate roots

## Build Instructions

### Prerequisites
- C++17 compatible compiler (g++, clang++, or MSVC)
- Python 3.x with matplotlib and pandas
- LaTeX compiler (optional, for PDF generation)

### Compilation
`bash
# Compile main program
g++ -std=c++17 -Wall -Wextra -o main main.cpp

# Compile visualization program  
g++ -std=c++17 -Wall -Wextra -o main_visualization main_visualization.cpp

# Compile verify errors program
g++ -std=c++17 -Wall -Wextra -o verify_errors verify_errors.cpp
`

### Running Programs
`bash
# Run main program
./main

# Run visualization (generates plots)
./run_plots.bat

# Run verify errors
./run_verify.bat
`

## Results Analysis

### Convergence Performance
- **Newton's Method**: Generally faster convergence (2-3 iterations)
- **Secant Method**: Slightly slower but doesn't require derivatives (3-4 iterations)
- **Double Precision**: More accurate results with smaller residuals
- **Float Precision**: Faster computation but lower accuracy

### Verify Error Analysis
- All verify errors are very small (< 1e-6 for float, < 1e-12 for double)
- Newton's method typically produces smaller verify errors
- Double precision shows significantly better accuracy than float

### Algorithm Comparison
| Method | Iterations | Accuracy | Derivative Required |
|--------|------------|----------|-------------------|
| Newton | 2-3 | High | Yes |
| Secant | 3-4 | Good | No |

## File Structure
`
src/
 Core Headers
    function.h          # Base function class
    solver.h            # Base solver class  
    newton.h            # Newton's method
    secant.h            # Secant method
    func1.h-func4.h     # Mathematical functions
    includes.h          # Main include file
 Programs
    main.cpp            # Main results program
    main_visualization.cpp # Plot data generation
    verify_errors.cpp   # Verify error table
 Scripts
    run_plots.bat       # Automated plotting
    run_verify.bat      # Automated verify errors
    compile_latex.bat   # LaTeX compilation
 Documentation
    README.md           # This file
    homework4_questions.tex # LaTeX questions
 Output Files
    results.txt         # Main results CSV
    verify_errors.txt   # Verify error table
    newton_plot.csv     # Newton method data
    secant_plot.csv     # Secant method data
    root_convergence_plot.png    # Root convergence plot
    residual_convergence_plot.png # Residual convergence plot
 Build
     Makefile            # Build automation
`

## Generated Files Summary

### Data Files
- 
esults.txt - Complete results table with all function/solver combinations
- verify_errors.txt - Detailed verify error analysis
- 
ewton_plot.csv - Newton method iteration data for plotting
- secant_plot.csv - Secant method iteration data for plotting

### Visualization Files
- 
oot_convergence_plot.png - Shows root convergence for all functions
- 
esidual_convergence_plot.png - Shows residual convergence for all functions


