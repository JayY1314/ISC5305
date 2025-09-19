# Homework 3 - 3D Grid Implementation

## Overview
This assignment implements three different approaches for storing and manipulating 3D arrays in C++:

1. **1D Array Method** (`grid3d_1d_array.cpp`) - Uses a single 1D array to represent 3D data
2. **Vector Method** (`grid3d_vector.cpp`) - Uses `std::vector<std::vector<std::vector<double>>>`
3. **New Operator Method** (`grid3d_new.cpp`) - Uses dynamically allocated 3D array with `double***`

## Implementation Details

### 1D Array Method (Grid1D)
- **Memory Layout**: Single contiguous block of memory
- **Index Mapping**: `index = i * ny * nz + j * nz + k`
- **Advantages**: 
  - Cache-friendly due to contiguous memory
  - Simple memory management
  - Fast access patterns
- **Disadvantages**:
  - Manual memory management required
  - Index calculation overhead

### Vector Method (GridVec)
- **Memory Layout**: Nested vector structure with potential memory fragmentation
- **Index Mapping**: `data[i][j][k]`
- **Advantages**:
  - Automatic memory management
  - Bounds checking (if enabled)
  - Easy to use
- **Disadvantages**:
  - Memory fragmentation
  - Slower access due to multiple indirections
  - Higher memory overhead

### New Operator Method (GridNew)
- **Memory Layout**: 3D array of pointers to pointers to arrays
- **Index Mapping**: `data[i][j][k]`
- **Advantages**:
  - Direct 3D array access
  - Flexible memory allocation
- **Disadvantages**:
  - Complex memory management
  - Memory fragmentation
  - Potential for memory leaks if not handled carefully

## Implemented Functions

All three classes implement the following interface:

### Constructors and Destructors
- `Grid(int nx, int ny, int nz)` - Constructor
- `~Grid()` - Destructor
- `Grid(const Grid& grid)` - Copy constructor
- `Grid& operator=(const Grid& grid)` - Assignment operator

### Access and Modification
- `double operator()(int i, int j, int k) const` - Access element (const)
- `void set(int i, int j, int k, double value)` - Set element value

### Information
- `int getSize() const` - Get total number of elements
- `int getMemory() const` - Get memory usage in bytes

### Arithmetic Operations
- `Grid operator+(const Grid& grid) const` - Addition
- `Grid operator*(double factor) const` - Scalar multiplication (member)
- `friend Grid operator*(double factor, const Grid& grid)` - Scalar multiplication (friend)
- `Grid& operator++()` - Prefix increment (increment all elements by 1)
- `Grid& operator+=(const Grid& grid)` - Addition assignment

### Output
- `friend std::ostream& operator<<(std::ostream& os, const Grid& grid)` - Output operator

## Memory Usage Analysis

### 1D Array Method
- **Data**: `nx * ny * nz * sizeof(double)` bytes
- **Overhead**: `3 * sizeof(int)` bytes for dimensions
- **Total**: `nx * ny * nz * 8 + 12` bytes (for 64-bit system)

### Vector Method
- **Data**: `nx * ny * nz * sizeof(double)` bytes
- **Vector Overhead**: `nx * sizeof(vector<vector<double>>) + nx * ny * sizeof(vector<double>)`
- **Total**: Approximately `nx * ny * nz * 8 + nx * 24 + nx * ny * 24 + 12` bytes

### New Operator Method
- **Data**: `nx * ny * nz * sizeof(double)` bytes
- **Pointer Overhead**: `nx * sizeof(double**) + nx * ny * sizeof(double*)`
- **Total**: `nx * ny * nz * 8 + nx * 8 + nx * ny * 8 + 12` bytes

## Performance Considerations

1. **Memory Access Pattern**: 1D array has the best cache locality
2. **Memory Management**: Vector method is safest, 1D array is most efficient
3. **Access Speed**: 1D array > New operator > Vector (generally)

## Compilation

To compile the project, use:
```bash
g++ -Wall -g -std=c++11 -o homework.x main.cpp grid3d_1d_array.cpp grid3d_vector.cpp grid3d_new.cpp
```

## Testing

The `main.cpp` file includes test functions for each grid type:
- `check_grid_1d_array(int nx, int ny, int nz)`
- `check_grid_vector(int nx, int ny, int nz)`
- `check_grid_new(int nx, int ny, int nz)`

Each test:
1. Creates a grid of specified dimensions
2. Prints grid size and memory usage
3. Fills the grid with test data (100*i + 10*j + k)
4. Prints the grid contents
5. Performs addition operation

## Answers to Graded Questions

### 1. How is a 1D array used to represent a 3D array in memory?
A 1D array represents a 3D array by flattening the 3D structure into a linear sequence. The mapping formula is: `index = i * ny * nz + j * nz + k`, where (i,j,k) are the 3D coordinates and (nx,ny,nz) are the dimensions.

### 2. Advantages and disadvantages of 1D array vs multiple vectors?
**Advantages of 1D array:**
- Better cache locality
- Simpler memory management
- Lower memory overhead
- Faster access patterns

**Disadvantages of 1D array:**
- Manual index calculation
- Less intuitive access pattern
- Manual memory management

### 3. Formula for accessing element at (i,j,k) in 1D array?
`index = i * ny * nz + j * nz + k`

### 4. Constructor implementation for 1D array?
```cpp
Grid1D::Grid1D(int nx_, int ny_, int nz_) : nx(nx_), ny(ny_), nz(nz_) {
    data = new double[nx * ny * nz];
    // Initialize to 0
    for (int i = 0; i < nx * ny * nz; i++) {
        data[i] = 0.0;
    }
}
```

### 5. Memory usage calculation for 1D array?
`sizeof(double) * nx * ny * nz + sizeof(int) * 3`

### 6. Memory allocation differences between the three methods?
- **1D Array**: Single `new double[size]` allocation
- **Vector**: Automatic memory management with `std::vector`
- **New Operator**: Multiple allocations: `new double**[nx]`, then `new double*[ny]` for each i, then `new double[nz]` for each (i,j)

### 7. Memory layout differences between vector and 1D array?
- **1D Array**: Contiguous memory block
- **Vector**: Fragmented memory with multiple indirections through vector objects

### 8. Initializing 3D vector structure?
```cpp
data.resize(nx);
for (int i = 0; i < nx; i++) {
    data[i].resize(ny);
    for (int j = 0; j < ny; j++) {
        data[i][j].resize(nz, 0.0);
    }
}
```

### 9. Accessing element in 3D vector?
`data[i][j][k]` - direct access through nested vectors

### 10. Destructor for dynamically allocated 3D array?
```cpp
GridNew::~GridNew() {
    if (data != nullptr) {
        for (int i = 0; i < nx; i++) {
            if (data[i] != nullptr) {
                for (int j = 0; j < ny; j++) {
                    delete[] data[i][j];
                }
                delete[] data[i];
            }
        }
        delete[] data;
        data = nullptr;
    }
}
```

### 11. Syntax for allocating/deallocating 3D array with new?
**Allocation:**
```cpp
data = new double**[nx];
for (int i = 0; i < nx; i++) {
    data[i] = new double*[ny];
    for (int j = 0; j < ny; j++) {
        data[i][j] = new double[nz];
    }
}
```

**Deallocation:**
```cpp
for (int i = 0; i < nx; i++) {
    for (int j = 0; j < ny; j++) {
        delete[] data[i][j];
    }
    delete[] data[i];
}
delete[] data;
```

### 12. Operator overloading role in C++?
Operator overloading allows classes to define custom behavior for operators, making code more intuitive and readable. For 3D grids, it enables natural syntax like `grid(i,j,k)` and `grid1 + grid2`.

### 13. Implementing operator() for 1D array?
```cpp
double Grid1D::operator()(int i, int j, int k) const {
    if (i < 0 || i >= nx || j < 0 || j >= ny || k < 0 || k >= nz) {
        throw std::out_of_range("Index out of bounds");
    }
    return data[i * ny * nz + j * nz + k];
}
```

### 14. Overloading + operator for 3D arrays?
```cpp
Grid Grid::operator+(const Grid& grid) const {
    if (nx != grid.nx || ny != grid.ny || nz != grid.nz) {
        throw std::invalid_argument("Dimensions must match");
    }
    Grid result(nx, ny, nz);
    for (int i = 0; i < nx; i++) {
        for (int j = 0; j < ny; j++) {
            for (int k = 0; k < nz; k++) {
                result.data[i][j][k] = data[i][j][k] + grid.data[i][j][k];
            }
        }
    }
    return result;
}
```

### 15. Overloading operator<< for printing?
```cpp
std::ostream& operator<<(std::ostream& os, const Grid& grid) {
    os << "Grid(" << grid.nx << "x" << grid.ny << "x" << grid.nz << "):\n";
    for (int i = 0; i < grid.nx; i++) {
        os << "Layer " << i << ":\n";
        for (int j = 0; j < grid.ny; j++) {
            for (int k = 0; k < grid.nz; k++) {
                os << grid(i, j, k) << " ";
            }
            os << "\n";
        }
        os << "\n";
    }
    return os;
}
```

### 16. Considerations for operator+ with dynamic arrays?
- Check for dimension compatibility
- Handle memory allocation for result
- Ensure proper cleanup in case of exceptions
- Use RAII principles

### 17. getMemory() function for vector method?
```cpp
int GridVec::getMemory() const {
    int vector_overhead = nx * sizeof(std::vector<std::vector<double>>) + 
                         nx * ny * sizeof(std::vector<double>) +
                         nx * ny * nz * sizeof(double);
    return vector_overhead + sizeof(int) * 3;
}
```

### 18. Testing different storage types?
Create identical test data and compare element-wise:
```cpp
bool gridsEqual = true;
for (int i = 0; i < nx && gridsEqual; i++) {
    for (int j = 0; j < ny && gridsEqual; j++) {
        for (int k = 0; k < nz && gridsEqual; k++) {
            if (abs(grid1(i,j,k) - grid2(i,j,k)) > tolerance) {
                gridsEqual = false;
            }
        }
    }
}
```

### 19. Memory issues with incorrect destructor?
- Memory leaks if not all allocated memory is freed
- Double deletion if destructor is called multiple times
- Dangling pointers if data is not set to nullptr
- Access to freed memory causing undefined behavior

### 20. Timing analysis comparison?
**Expected Performance (best to worst):**
1. **1D Array**: Best cache locality, single memory access
2. **New Operator**: Direct access but potential cache misses
3. **Vector**: Multiple indirections, worst cache performance

**Factors affecting performance:**
- Cache locality
- Memory access patterns
- Compiler optimizations
- System architecture

## Conclusion

The 1D array method provides the best performance for most use cases due to its cache-friendly memory layout and simple access patterns. The vector method offers the best safety and ease of use, while the new operator method provides the most direct 3D array semantics but requires careful memory management.

All three implementations provide the same interface, making them interchangeable for different performance and safety requirements.
