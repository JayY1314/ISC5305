# Homework 1 Report - Answers to Questions

## 1. Command-line I/O

**How your program uses argc/argv to accept the name of the input parameter file:**

The program uses the standard C++ main function signature:
`cpp
int main(int argc, char* argv[])
`

The program checks for exactly one command-line argument (the parameter file name):
`cpp
if (argc != 2) {
    std::cerr << "Usage: " << argv[0] << " <parameter_file>" << std::endl;
    return 1;
}
`

**User error cases guarded against:**
- **Missing filename**: Checked with rgc != 2 - program exits with usage message
- **Unreadable file**: Checked with if (!file.is_open()) when opening parameter file
- **Invalid parameter file format**: Program validates that all required parameters are present
- **Data file not found**: Checked with if (!file.is_open()) when opening data file
- **Invalid data format**: Try-catch blocks around std::stod() for parsing numeric values

## 2. Parameter file design

**Format of parameter file:**
The parameter file uses a simple key-value format:
`
data_file = AbaloneAge.csv
num_lines = 1000
column = 0
`

**How code parses them:**
`cpp
std::istringstream iss(line);
std::string key, value;
if (std::getline(iss, key, '=') && std::getline(iss, value)) {
    key.erase(std::remove_if(key.begin(), key.end(), ::isspace), key.end());
    value.erase(std::remove_if(value.begin(), value.end(), ::isspace), value.end());
    // Process key-value pairs...
}
`

**Why parameter file is preferable:**
- **Flexibility**: Easy to change parameters without recompiling
- **Reusability**: Same program can process different datasets
- **Maintainability**: Parameters are clearly separated from code logic
- **Automation**: Enables batch processing with different parameter sets

## 3. Data ingestion + brief description

**1-D dataset chosen:**
- **AbaloneAge.csv**: Length measurements of abalone shells (first column)
- **StressLevel.csv**: Anxiety level measurements from students (first column)

**Why more interesting than temperature:**
- **Biological significance**: Abalone length relates to age and growth patterns
- **Psychological relevance**: Anxiety levels show human behavioral data
- **Diverse scales**: Different measurement units and ranges
- **Real-world applications**: Both datasets have practical implications

**1-2 line description communicated:**
- **Abalone**: "Length measurements of abalone shells showing growth patterns over time"
- **Stress**: "Anxiety level measurements from students indicating stress distribution"

## 4. Statistics: mean/variance/std-dev

**Formulas used:**
- **Mean**: μ = (1/n)  Σ(xᵢ)
- **Variance**: σ = (1/(n-1))  Σ(xᵢ - μ) (Bessel's correction)
- **Standard Deviation**: σ = (variance)

**Implementation approach:**
Used two-pass method for numerical stability:
`cpp
double mean = calculateMean(data);
double stdDev = calculateStdDev(data, mean);
`

**Why two-pass method:**
- **Numerical stability**: Avoids accumulation of rounding errors
- **Accuracy**: More precise for large datasets or ill-scaled data
- **Clarity**: Separates mean calculation from variance calculation

**Single-pass variance pitfalls:**
- **Numerical instability**: Can lose precision with large numbers
- **Overflow risk**: Sum of squares can become very large
- **Accuracy loss**: Rounding errors accumulate in single pass

## 5. Output formatting with <iomanip>

**Printing floating-point to two decimal places:**
`cpp
std::cout << std::fixed << std::setprecision(2) << value;
`

**Difference between std::fixed and std::scientific:**
- **std::fixed**: Displays numbers in fixed-point notation (e.g., 123.45)
- **std::scientific**: Displays numbers in scientific notation (e.g., 1.23e+02)

**Interaction with std::setprecision:**
- **std::fixed**: setprecision(2) means exactly 2 digits after decimal point
- **std::scientific**: setprecision(2) means 2 significant digits total
- **Default**: setprecision(2) means 2 significant digits total

## 6. Data structures: std::vector

**Why std::vector<double> is appropriate:**
- **Dynamic sizing**: Can handle variable amounts of data
- **Type safety**: Compile-time type checking
- **Memory management**: Automatic allocation/deallocation
- **STL compatibility**: Works with algorithms and iterators

**Contrast with raw array:**
- **Safety**: vector has bounds checking with at(), arrays don't
- **Usability**: vector has size(), push_back(), etc.
- **Memory**: vector manages memory automatically
- **Flexibility**: vector can grow/shrink dynamically

**Amortized complexity of push_back:**
- **O(1) amortized**: Most operations are O(1), occasional reallocation is O(n)
- **reserve() helpful**: When you know approximate size, prevents multiple reallocations

## 7. Timing the code

**C++ facilities used:**
`cpp
#include <chrono>
auto start = std::chrono::high_resolution_clock::now();
// ... work ...
auto end = std::chrono::high_resolution_clock::now();
auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
`

**What is being timed:**
- **End-to-end timing**: Complete program execution including I/O and computation
- **Reported as**: Milliseconds for human readability

**Alternative approaches:**
- **std::chrono::steady_clock**: For measuring elapsed time (not affected by system clock changes)
- **Separate timing**: Could time I/O and computation separately for analysis

## 8. Batch runs over a range of parameters

**Automation method:**
Created batch script (un_processing.bat) that:
`atch
g++ -o data_processor data_processor.cpp
data_processor.exe abalone_params.txt
data_processor.exe stress_params.txt
`

**Naming convention for outputs:**
- **Parameter-based naming**: {datafile}_normalized.txt
- **Example**: AbaloneAge_normalized.txt, StressLevel_normalized.txt
- **Prevents overwriting**: Each dataset gets unique output file

**Alternative approaches:**
- **Makefile phony targets**: Could use make run target
- **Shell script loops**: Could iterate over parameter files
- **Program-internal loops**: Could process multiple files in one run

## 9. Build system and IDE

**Makefile organization:**
`makefile
all: data_processor
data_processor: data_processor.cpp
    g++ -o data_processor data_processor.cpp
clean:
    rm -f data_processor *.exe
run: data_processor
    ./data_processor abalone_params.txt
    ./data_processor stress_params.txt
`

**Main targets:**
- **all**: Default target, builds the executable
- **data_processor**: Compiles the C++ program
- **clean**: Removes generated files
- **run**: Executes the program with both parameter files

**IDE configuration (VS Code/Cursor):**
- **Build task**: Configured to run make command
- **Program arguments**: Set in launch.json or tasks.json
- **Debugging**: Can set breakpoints and step through code
- **IntelliSense**: Provides code completion and error detection

## 10. Plotting pipeline and artifacts

**File format produced:**
- **Input format**: CSV files with 'value' column containing normalized data
- **Example**: AbaloneAge_normalized.csv, StressLevel_normalized.csv

**Python code reading files:**
`python
def read_series(path, label):
    df = pd.read_csv(path)
    return df['value'], label
`

**Verification of curve correspondence:**
- **File naming**: Clear naming convention links curves to datasets
- **Legend labels**: Each curve labeled as "Series 1", "Series 2", etc.
- **Data validation**: Checked that data ranges are [0,1] as expected
- **Visual inspection**: Plotted data shows expected patterns for each dataset

**Output artifacts:**
- **3-5 output files**: Parameter files, normalized data files, plot script
- **3-5 curves**: One curve per processed dataset
- **Labeled axes**: "Index" (x-axis), "Normalized Value" (y-axis)
- **Title**: "Normalized 1-D Data (MinMax to [0,1])"
- **Legend**: Shows which curve corresponds to which dataset

## Summary

This implementation successfully addresses all homework requirements:
-  Command-line argument processing with error handling
-  Parameter file parsing and validation
-  Statistical calculations with proper formulas
-  Data normalization to [0,1] range
-  Proper output formatting with iomanip
-  Vector-based data structures
-  Timing measurements
-  Batch processing automation
-  Comprehensive plotting pipeline
-  Complete documentation and error handling

## Data Processing Results (Updated for 1000 samples)

### AbaloneAge Data (Length column, 1000 samples)
- **Mean**: Calculated from 1000 data points
- **Standard Deviation**: Calculated from 1000 data points
- **Data Range**: Normalized to [0,1] range

### StressLevel Data (anxiety_level column, 1000 samples)
- **Mean**: Calculated from 1000 data points
- **Standard Deviation**: Calculated from 1000 data points
- **Data Range**: Normalized to [0,1] range
