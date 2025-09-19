/*
1) Create a 3D grid in 3 different ways:

The object is a class: Grid
Size in each direction: nx, ny, nz

Method 1)
*/
#ifndef __GRID3D_1D_ARRAY_H__
#define __GRID3D_1D_ARRAY_H__

#include <iostream>

class Grid1D {
public:
    Grid1D(int nx_, int ny_, int nz_);
    ~Grid1D();
    Grid1D(const Grid1D& grid);
    Grid1D& operator=(const Grid1D& grid);
    int getSize() const;
    int getMemory() const;
    // Get a value
    double operator()(int i, int j, int k) const;
    // Set a value. Using operator() is more elegant, but requires
    // more knowledge to implement
    void set(int i, int j, int k, double value);
    Grid1D operator+(const Grid1D& grid) const;
    Grid1D operator*(double factor) const;
    friend Grid1D operator*(double factor, const Grid1D& grid);
    // Prefix increment: increment every element in the grid by 1
    Grid1D& operator++();
    Grid1D& operator+=(const Grid1D& grid);
    friend std::ostream& operator<<(std::ostream& os, const Grid1D& grid);

private:
    double* data;
    int nx, ny, nz;
};

#endif
