# Polynomial Binary Tree Implementation

## Overview
This project implements a C++ program for representing, manipulating, and differentiating polynomials using binary expression trees. The implementation follows modern C++ design principles and STL integration.

## Requirements
- C++17 or later
- g++ compiler with C++17 support
- Python 3 with numpy and matplotlib (for visualization)

## Building the Project

### Compilation
Compile the project using the following command:

```bash
g++ -std=c++17 -Wall -Wextra -pedantic -o polynomial Node.cpp PolynomialBinaryTree.cpp main.cpp
```

This will create an executable named `polynomial`.

### Compilation Options
- `-std=c++17`: Uses C++17 standard
- `-Wall`: Enables all warnings
- `-Wextra`: Enables extra warnings
- `-pedantic`: Strict ISO C++ compliance

## Running the Program

### Execute the C++ Program
```bash
./polynomial
```

This will:
1. Create a polynomial from zeros: `[-0.86, -0.34, 0.34, 0.86]` using `from_zeros()`
2. Evaluate the polynomial and its first 5 derivatives at 100 points in the interval [-1, 1]
3. Generate `polynomial_evaluation.txt` with the results

### Run Python Visualization
After running the C++ program, execute the Python script to generate the plot:

```bash
python polynomial_plot.py
```

This will:
1. Load data from `polynomial_evaluation.txt`
2. Plot the polynomial and its derivatives
3. Save the plot as `polynomial_plot.pdf`

## Project Structure

- `Node.h` / `Node.cpp`: Node class for expression tree nodes
- `PolynomialBinaryTree.h` / `PolynomialBinaryTree.cpp`: Main polynomial class with operations
- `main.cpp`: Main program with `from_zeros()` and evaluation
- `polynomial_plot.py`: Python script for visualization
- `polynomial_evaluation.txt`: Output file with evaluation results (generated)
- `polynomial_plot.pdf`: Visualization output (generated)

## Features

### Part 1: Node Structure
- NodeType enum (Constant, Variable, Operator)
- Node class with variant value storage
- Smart pointer-based memory management

### Part 2: Polynomial Binary Tree
- Operator overloading (+, -, *)
- Recursive expression construction
- Evaluation and string conversion
- Tree traversal methods

### Part 3: Differentiation
- Recursive differentiation
- Expression simplification
- Zero term removal and constant combination

### Part 4: Polynomial Definition and Evaluation
- `from_zeros()` function to construct polynomials from zeros
- Multiple derivative evaluation using symbolic differentiation
- File output with formatted results (4 decimal places)

### Part 5: Python Visualization
- Data loading and plotting
- Multiple derivative visualization
- PDF export

### Part 6: STL Integration
- ForwardIterator implementation
- STL algorithm compatibility
- In-order traversal

## Notes

- The program uses `std::shared_ptr` for memory management to allow node sharing in expression trees
- All floating-point numbers are formatted to 4 decimal places in the output file
- The implementation supports symbolic differentiation with automatic simplification

