#include "grid3d_new.h"
#include <stdexcept>

// Constructor: allocate memory using new
GridNew::GridNew(int nx_, int ny_, int nz_) : nx(nx_), ny(ny_), nz(nz_) {
    // Allocate 3D array using new
    data = new double**[nx];
    for (int i = 0; i < nx; i++) {
        data[i] = new double*[ny];
        for (int j = 0; j < ny; j++) {
            data[i][j] = new double[nz];
            // Initialize to 0
            for (int k = 0; k < nz; k++) {
                data[i][j][k] = 0.0;
            }
        }
    }
}

// Destructor: free allocated memory
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

// Copy constructor
GridNew::GridNew(const GridNew& grid) : nx(grid.nx), ny(grid.ny), nz(grid.nz) {
    // Allocate new memory
    data = new double**[nx];
    for (int i = 0; i < nx; i++) {
        data[i] = new double*[ny];
        for (int j = 0; j < ny; j++) {
            data[i][j] = new double[nz];
            // Copy data
            for (int k = 0; k < nz; k++) {
                data[i][j][k] = grid.data[i][j][k];
            }
        }
    }
}

// Assignment operator
GridNew& GridNew::operator=(const GridNew& grid) {
    if (this != &grid) { // Check for self-assignment
        // Free existing memory
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
        }
        
        // Copy dimensions
        nx = grid.nx;
        ny = grid.ny;
        nz = grid.nz;
        
        // Allocate new memory and copy data
        data = new double**[nx];
        for (int i = 0; i < nx; i++) {
            data[i] = new double*[ny];
            for (int j = 0; j < ny; j++) {
                data[i][j] = new double[nz];
                for (int k = 0; k < nz; k++) {
                    data[i][j][k] = grid.data[i][j][k];
                }
            }
        }
    }
    return *this;
}

// Get total number of elements
int GridNew::getSize() const {
    return nx * ny * nz;
}

// Get memory usage in bytes
int GridNew::getMemory() const {
    // Calculate memory for the 3D array structure
    int array_memory = nx * sizeof(double**) + 
                      nx * ny * sizeof(double*) +
                      nx * ny * nz * sizeof(double);
    return array_memory + sizeof(int) * 3; // + dimensions
}

// Access element at (i,j,k) - const version
double GridNew::operator()(int i, int j, int k) const {
    if (i < 0 || i >= nx || j < 0 || j >= ny || k < 0 || k >= nz) {
        throw std::out_of_range("Index out of bounds");
    }
    return data[i][j][k];
}

// Set element at (i,j,k)
void GridNew::set(int i, int j, int k, double value) {
    if (i < 0 || i >= nx || j < 0 || j >= ny || k < 0 || k >= nz) {
        throw std::out_of_range("Index out of bounds");
    }
    data[i][j][k] = value;
}

// Addition operator
GridNew GridNew::operator+(const GridNew& grid) const {
    if (nx != grid.nx || ny != grid.ny || nz != grid.nz) {
        throw std::invalid_argument("Grid dimensions must match for addition");
    }
    
    GridNew result(nx, ny, nz);
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
GridNew GridNew::operator*(double factor) const {
    GridNew result(nx, ny, nz);
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
GridNew operator*(double factor, const GridNew& grid) {
    return grid * factor;
}

// Prefix increment: increment every element by 1
GridNew& GridNew::operator++() {
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
GridNew& GridNew::operator+=(const GridNew& grid) {
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
std::ostream& operator<<(std::ostream& os, const GridNew& grid) {
    os << "GridNew(" << grid.nx << "x" << grid.ny << "x" << grid.nz << "):\n";
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
