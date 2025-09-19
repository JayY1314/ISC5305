/*
1) Create a 3D grid in 3 different ways:

The object is a class: Grid
Size in each direction: nx, ny, nz

Method 1)
*/
#ifndef __GRID3D_VECTOR_H__
#define __GRID3D_VECTOR_H__

#include <iostream>
#include <vector>

class GridVec {
public:
    GridVec(int nx_ = 1, int ny_ = 1, int nz_ = 1);
    ~GridVec();
    GridVec(const GridVec& grid);
    GridVec& operator=(const GridVec& grid);
    int getSize() const;
    int getMemory() const;
    // Get a value
    double operator()(int i, int j, int k) const;
    // Set a value. Using operator() is more elegant, but requires
    // more knowledge to implement
    void set(int i, int j, int k, double value);
    GridVec operator+(const GridVec& grid) const;
    GridVec operator*(double factor) const;
    friend GridVec operator*(double factor, const GridVec& grid);
    GridVec& operator++();
    GridVec& operator+=(const GridVec& grid);
    friend std::ostream& operator<<(std::ostream& os, const GridVec& grid);

private:
    std::vector<std::vector<std::vector<double> > > data;
    int nx, ny, nz;
};

#endif
