# ScientificContainer Project

## Project Overview

This project implements a comprehensive scientific computing container system using modern C++ features including templates, polymorphism, exception handling, and advanced STL containers. The system provides a flexible framework for managing scientific objects with metadata, serialization, and various computational capabilities.

## Features

- **Template-based Container**: Generic `ScientificContainer<T>` supporting different key types
- **Polymorphic Objects**: Abstract base class `ScientificObject` with derived classes
- **Exception Safety**: Custom `DuplicateElementException` with proper inheritance
- **Metadata Management**: Executable callbacks and metadata storage
- **Template Specialization**: Specialized `process_data` functions for different types
- **Lambda-based Filtering**: Functional programming support for data filtering
- **Range-based Iteration**: Modern C++ iterator support
- **JSON Serialization**: Export container data to JSON format

## File Dependencies

- **main.cpp** depends on **code.cpp** (includes it automatically)
- **public_tests.cpp** depends on **code.cpp** (includes it automatically)
- **doc_tests (1).cpp** depends on **code.cpp** (includes it automatically)

## File Structure

```
HW6/
├── code.cpp                           # Core implementation (classes, methods)
├── main.cpp                           # Test functions + includes code.cpp
├── public_tests.cpp                   # Public test suite
├── doc_tests (1).cpp                  # Detailed test suite
├── Makefile                           # Build configuration
└── README.md                          # This file
```

## Build Instructions

### Prerequisites

- **Visual Studio 2022** (Community, Professional, or Enterprise)
- **Windows 10/11** with Developer Command Prompt
- **C++17** support

### Using Makefile (Recommended)

1. **Build Options:**
   ```cmd
   # Build main integrated executable (recommended)
   make all
   
   # Build public tests
   make public
   
   # Build detailed test suite
   make doc-tests
   ```

2. **Run Options:**
   ```cmd
   # Run main integrated executable (all 8 tasks)
   make run
   
   # Run public tests
   make run-public
   
   # Run detailed tests
   make run-doc-tests
   ```

### Manual Compilation

1. **Build Main Executable:**
   ```cmd
   cl main.cpp /EHsc /std:c++17 /Fe:main.exe
   # Note: main.cpp automatically includes code.cpp
   ```

2. **Build Public Tests:**
   ```cmd
   cl public_tests.cpp /EHsc /std:c++17 /Fe:public_test.exe
   ```

3. **Build Detailed Tests:**
   ```cmd
   cl "doc_tests (1).cpp" /EHsc /std:c++17 /Fe:doc_test.exe
   ```

## Usage

### Running the Integrated Test Suite

```cmd
# Run the complete test suite
./main.exe
```

This will execute all 8 tasks with comprehensive testing. The main.cpp file includes all test functions and automatically includes the core implementation from code.cpp:
- Task 1: TensorCalculation functionality
- Task 2: Metadata with executable callbacks
- Task 3: DuplicateElementException handling
- Task 4: String processing with process_data
- Task 5: Complex number processing
- Task 6: Lambda-based filtering
- Task 7: Range-based for loop support
- Task 8: JSON serialization

### Running Other Test Suites

```cmd
# Run public test suite
./public_test.exe

# Run detailed test suite
./doc_test.exe
```

## Generated Files

The program generates several output files:
- `test_container.json` - Main container serialization
- `empty_container.json` - Empty container test
- `string_container.json` - String key container test
- `sci_container.json` - Default serialization output

## Clean Up

```cmd
# Remove all generated files
make clean

# Remove only executables
make clean-exe

# Remove only object files
make clean-obj

# Remove only JSON files
make clean-json
```

## Task Descriptions

### Task 1: TensorCalculation
Tests the basic functionality of the `TensorCalculation` class and its integration with `ScientificContainer`.

### Task 2: Metadata with Executable Callbacks
Demonstrates metadata management with executable callback functions that are triggered during metadata printing.

### Task 3: DuplicateElementException
Tests custom exception handling for duplicate element insertion with proper inheritance from `std::runtime_error`.

### Task 4: String Processing
Tests the `process_data` function overload for `std::string` type, including string reversal functionality.

### Task 5: Complex Number Processing
Tests template specialization for `std::complex<double>` with real and imaginary part processing.

### Task 6: Lambda-based Filtering
Tests functional programming capabilities with lambda-based element filtering.

### Task 7: Range-based For Loop Support
Tests modern C++ iterator support and range-based for loop functionality.

### Task 8: JSON Serialization
Tests JSON-like serialization of container data with proper formatting and structure.

## Technical Features

- **Template Programming**: Generic container supporting multiple key types
- **Exception Safety**: Strong exception safety guarantees
- **Memory Management**: Smart pointer usage with proper RAII
- **Modern C++**: C++17 features including structured bindings and `if constexpr`
- **STL Integration**: Extensive use of standard library containers and algorithms
- **Performance**: Optimized for scientific computing workloads

## Troubleshooting

### Common Issues

1. **Compiler Not Found:**
   - Ensure Visual Studio Developer Command Prompt is used
   - Verify Visual Studio 2022 installation

2. **C++17 Not Supported:**
   - Update to Visual Studio 2017 or later
   - Use `/std:c++17` flag

3. **Missing Headers:**
   - Ensure all required STL headers are included
   - Check for typos in include statements


## License

This project is part of academic coursework for ISC5305-SP Advanced Programming at FSU.
