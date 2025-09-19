#include "grid3d_1d_array.h"
#include <cstring>
#include <stdexcept>

// Constructor: allocate memory for 1D array
Grid1D::Grid1D(int nx_, int ny_, int nz_) : nx(nx_), ny(ny_), nz(nz_) {
    data = new double[nx * ny * nz];
    // Initialize all elements to 0
    for (int i = 0; i < nx * ny * nz; i++) {
        data[i] = 0.0;
    }
}

// Destructor: free allocated memory
Grid1D::~Grid1D() {
    delete[] data;
}

// Copy constructor
Grid1D::Grid1D(const Grid1D& grid) : nx(grid.nx), ny(grid.ny), nz(grid.nz) {
    data = new double[nx * ny * nz];
    // Copy all elements
    for (int i = 0; i < nx * ny * nz; i++) {
        data[i] = grid.data[i];
    }
}

// Assignment operator
Grid1D& Grid1D::operator=(const Grid1D& grid) {
    if (this != &grid) { // Check for self-assignment
        // Delete old data
        delete[] data;
        
        // Copy dimensions
        nx = grid.nx;
        ny = grid.ny;
        nz = grid.nz;
        
        // Allocate new memory and copy data
        data = new double[nx * ny * nz];
        for (int i = 0; i < nx * ny * nz; i++) {
            data[i] = grid.data[i];
        }
    }
    return *this;
}

// Get total number of elements
int Grid1D::getSize() const {
    return nx * ny * nz;
}

// Get memory usage in bytes
int Grid1D::getMemory() const {
    return sizeof(double) * nx * ny * nz + sizeof(int) * 3; // data + dimensions
}

// Access element at (i,j,k) - const version
double Grid1D::operator()(int i, int j, int k) const {
    if (i < 0 || i >= nx || j < 0 || j >= ny || k < 0 || k >= nz) {
        throw std::out_of_range("Index out of bounds");
    }
    return data[i * ny * nz + j * nz + k];
}

// Set element at (i,j,k)
void Grid1D::set(int i, int j, int k, double value) {
    if (i < 0 || i >= nx || j < 0 || j >= ny || k < 0 || k >= nz) {
        throw std::out_of_range("Index out of bounds");
    }
    data[i * ny * nz + j * nz + k] = value;
}

// Addition operator
Grid1D Grid1D::operator+(const Grid1D& grid) const {
    if (nx != grid.nx || ny != grid.ny || nz != grid.nz) {
        throw std::invalid_argument("Grid dimensions must match for addition");
    }
    
    Grid1D result(nx, ny, nz);
    for (int i = 0; i < nx * ny * nz; i++) {
        result.data[i] = data[i] + grid.data[i];
    }
    return result;
}

// Multiplication by scalar (member function)
Grid1D Grid1D::operator*(double factor) const {
    Grid1D result(nx, ny, nz);
    for (int i = 0; i < nx * ny * nz; i++) {
        result.data[i] = data[i] * factor;
    }
    return result;
}

// Multiplication by scalar (friend function)
Grid1D operator*(double factor, const Grid1D& grid) {
    return grid * factor;
}

// Prefix increment: increment every element by 1
Grid1D& Grid1D::operator++() {
    for (int i = 0; i < nx * ny * nz; i++) {
        data[i] += 1.0;
    }
    return *this;
}

// Addition assignment operator
Grid1D& Grid1D::operator+=(const Grid1D& grid) {
    if (nx != grid.nx || ny != grid.ny || nz != grid.nz) {
        throw std::invalid_argument("Grid dimensions must match for addition");
    }
    
    for (int i = 0; i < nx * ny * nz; i++) {
        data[i] += grid.data[i];
    }
    return *this;
}

// Output operator
std::ostream& operator<<(std::ostream& os, const Grid1D& grid) {
    os << "Grid1D(" << grid.nx << "x" << grid.ny << "x" << grid.nz << "):\n";
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
