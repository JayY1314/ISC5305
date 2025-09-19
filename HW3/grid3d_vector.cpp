#include "grid3d_vector.h"
#include <stdexcept>

// Constructor: initialize 3D vector structure
GridVec::GridVec(int nx_, int ny_, int nz_) : nx(nx_), ny(ny_), nz(nz_) {
    data.resize(nx);
    for (int i = 0; i < nx; i++) {
        data[i].resize(ny);
        for (int j = 0; j < ny; j++) {
            data[i][j].resize(nz, 0.0);
        }
    }
}

// Destructor: vector automatically handles memory cleanup
GridVec::~GridVec() {
    // No explicit cleanup needed - vector handles it
}

// Copy constructor
GridVec::GridVec(const GridVec& grid) : nx(grid.nx), ny(grid.ny), nz(grid.nz) {
    data = grid.data; // Vector copy constructor handles deep copy
}

// Assignment operator
GridVec& GridVec::operator=(const GridVec& grid) {
    if (this != &grid) { // Check for self-assignment
        nx = grid.nx;
        ny = grid.ny;
        nz = grid.nz;
        data = grid.data; // Vector assignment handles deep copy
    }
    return *this;
}

// Get total number of elements
int GridVec::getSize() const {
    return nx * ny * nz;
}

// Get memory usage in bytes
int GridVec::getMemory() const {
    // Calculate memory for the vector structure
    int vector_overhead = nx * sizeof(std::vector<std::vector<double>>) + 
                         nx * ny * sizeof(std::vector<double>) +
                         nx * ny * nz * sizeof(double);
    return vector_overhead + sizeof(int) * 3; // + dimensions
}

// Access element at (i,j,k) - const version
double GridVec::operator()(int i, int j, int k) const {
    if (i < 0 || i >= nx || j < 0 || j >= ny || k < 0 || k >= nz) {
        throw std::out_of_range("Index out of bounds");
    }
    return data[i][j][k];
}

// Set element at (i,j,k)
void GridVec::set(int i, int j, int k, double value) {
    if (i < 0 || i >= nx || j < 0 || j >= ny || k < 0 || k >= nz) {
        throw std::out_of_range("Index out of bounds");
    }
    data[i][j][k] = value;
}

// Addition operator
GridVec GridVec::operator+(const GridVec& grid) const {
    if (nx != grid.nx || ny != grid.ny || nz != grid.nz) {
        throw std::invalid_argument("Grid dimensions must match for addition");
    }
    
    GridVec result(nx, ny, nz);
    for (int i = 0; i < nx; i++) {
        for (int j = 0; j < ny; j++) {
            for (int k = 0; k < nz; k++) {
                result.data[i][j][k] = data[i][j][k] + grid.data[i][j][k];
            }
        }
    }
    return result;
}

// Multiplication by scalar (member function)
GridVec GridVec::operator*(double factor) const {
    GridVec result(nx, ny, nz);
    for (int i = 0; i < nx; i++) {
        for (int j = 0; j < ny; j++) {
            for (int k = 0; k < nz; k++) {
                result.data[i][j][k] = data[i][j][k] * factor;
            }
        }
    }
    return result;
}

// Multiplication by scalar (friend function)
GridVec operator*(double factor, const GridVec& grid) {
    return grid * factor;
}

// Prefix increment: increment every element by 1
GridVec& GridVec::operator++() {
    for (int i = 0; i < nx; i++) {
        for (int j = 0; j < ny; j++) {
            for (int k = 0; k < nz; k++) {
                data[i][j][k] += 1.0;
            }
        }
    }
    return *this;
}

// Addition assignment operator
GridVec& GridVec::operator+=(const GridVec& grid) {
    if (nx != grid.nx || ny != grid.ny || nz != grid.nz) {
        throw std::invalid_argument("Grid dimensions must match for addition");
    }
    
    for (int i = 0; i < nx; i++) {
        for (int j = 0; j < ny; j++) {
            for (int k = 0; k < nz; k++) {
                data[i][j][k] += grid.data[i][j][k];
            }
        }
    }
    return *this;
}

// Output operator
std::ostream& operator<<(std::ostream& os, const GridVec& grid) {
    os << "GridVec(" << grid.nx << "x" << grid.ny << "x" << grid.nz << "):\n";
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
