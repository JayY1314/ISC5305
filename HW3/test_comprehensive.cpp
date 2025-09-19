#include "grid3d_1d_array.h"
#include "grid3d_vector.h"
#include "grid3d_new.h"
#include <iostream>
#include <cassert>
#include <chrono>

using namespace std;

// Test function for Grid1D
void test_grid1d() {
    cout << "=== Testing Grid1D (1D Array) ===" << endl;
    
    // Test constructor
    Grid1D grid(2, 3, 4);
    assert(grid.getSize() == 24);
    cout << " Constructor test passed" << endl;
    
    // Test set and get
    grid.set(0, 0, 0, 1.5);
    assert(grid(0, 0, 0) == 1.5);
    cout << " Set/Get test passed" << endl;
    
    // Test copy constructor
    Grid1D grid2(grid);
    assert(grid2(0, 0, 0) == 1.5);
    cout << " Copy constructor test passed" << endl;
    
    // Test assignment
    Grid1D grid3(2, 3, 4);
    grid3 = grid;
    assert(grid3(0, 0, 0) == 1.5);
    cout << " Assignment test passed" << endl;
    
    // Test addition
    Grid1D grid4(2, 3, 4);
    grid4.set(0, 0, 0, 2.0);
    Grid1D sum = grid + grid4;
    assert(sum(0, 0, 0) == 3.5);
    cout << " Addition test passed" << endl;
    
    // Test scalar multiplication
    Grid1D mult = grid * 2.0;
    assert(mult(0, 0, 0) == 3.0);
    cout << " Scalar multiplication test passed" << endl;
    
    // Test increment
    ++grid;
    assert(grid(0, 0, 0) == 2.5);
    cout << " Increment test passed" << endl;
    
    // Test += operator
    grid += grid4;
    assert(grid(0, 0, 0) == 4.5);
    cout << " += operator test passed" << endl;
    
    cout << "Grid1D memory usage: " << grid.getMemory() << " bytes" << endl;
    cout << "All Grid1D tests passed!" << endl << endl;
}

// Test function for GridVec
void test_gridvec() {
    cout << "=== Testing GridVec (Vector) ===" << endl;
    
    // Test constructor
    GridVec grid(2, 3, 4);
    assert(grid.getSize() == 24);
    cout << " Constructor test passed" << endl;
    
    // Test set and get
    grid.set(0, 0, 0, 1.5);
    assert(grid(0, 0, 0) == 1.5);
    cout << " Set/Get test passed" << endl;
    
    // Test copy constructor
    GridVec grid2(grid);
    assert(grid2(0, 0, 0) == 1.5);
    cout << " Copy constructor test passed" << endl;
    
    // Test assignment
    GridVec grid3(2, 3, 4);
    grid3 = grid;
    assert(grid3(0, 0, 0) == 1.5);
    cout << " Assignment test passed" << endl;
    
    // Test addition
    GridVec grid4(2, 3, 4);
    grid4.set(0, 0, 0, 2.0);
    GridVec sum = grid + grid4;
    assert(sum(0, 0, 0) == 3.5);
    cout << " Addition test passed" << endl;
    
    // Test scalar multiplication
    GridVec mult = grid * 2.0;
    assert(mult(0, 0, 0) == 3.0);
    cout << " Scalar multiplication test passed" << endl;
    
    // Test increment
    ++grid;
    assert(grid(0, 0, 0) == 2.5);
    cout << " Increment test passed" << endl;
    
    // Test += operator
    grid += grid4;
    assert(grid(0, 0, 0) == 4.5);
    cout << " += operator test passed" << endl;
    
    cout << "GridVec memory usage: " << grid.getMemory() << " bytes" << endl;
    cout << "All GridVec tests passed!" << endl << endl;
}

// Test function for GridNew
void test_gridnew() {
    cout << "=== Testing GridNew (New Operator) ===" << endl;
    
    // Test constructor
    GridNew grid(2, 3, 4);
    assert(grid.getSize() == 24);
    cout << " Constructor test passed" << endl;
    
    // Test set and get
    grid.set(0, 0, 0, 1.5);
    assert(grid(0, 0, 0) == 1.5);
    cout << " Set/Get test passed" << endl;
    
    // Test copy constructor
    GridNew grid2(grid);
    assert(grid2(0, 0, 0) == 1.5);
    cout << " Copy constructor test passed" << endl;
    
    // Test assignment
    GridNew grid3(2, 3, 4);
    grid3 = grid;
    assert(grid3(0, 0, 0) == 1.5);
    cout << " Assignment test passed" << endl;
    
    // Test addition
    GridNew grid4(2, 3, 4);
    grid4.set(0, 0, 0, 2.0);
    GridNew sum = grid + grid4;
    assert(sum(0, 0, 0) == 3.5);
    cout << " Addition test passed" << endl;
    
    // Test scalar multiplication
    GridNew mult = grid * 2.0;
    assert(mult(0, 0, 0) == 3.0);
    cout << " Scalar multiplication test passed" << endl;
    
    // Test increment
    ++grid;
    assert(grid(0, 0, 0) == 2.5);
    cout << " Increment test passed" << endl;
    
    // Test += operator
    grid += grid4;
    assert(grid(0, 0, 0) == 4.5);
    cout << " += operator test passed" << endl;
    
    cout << "GridNew memory usage: " << grid.getMemory() << " bytes" << endl;
    cout << "All GridNew tests passed!" << endl << endl;
}

// Performance test function
void performance_test() {
    cout << "=== Performance Test ===" << endl;
    
    const int sizes[] = {10, 100, 1000};
    const int num_tests = 5;
    
    for (int size : sizes) {
        cout << "Testing with size " << size << "x" << size << "x" << size << endl;
        
        // Test Grid1D
        auto start = chrono::high_resolution_clock::now();
        for (int test = 0; test < num_tests; test++) {
            Grid1D grid1(size, size, size);
            Grid1D grid2(size, size, size);
            
            // Fill with test data
            for (int i = 0; i < size; i++) {
                for (int j = 0; j < size; j++) {
                    for (int k = 0; k < size; k++) {
                        grid1.set(i, j, k, i + j + k);
                        grid2.set(i, j, k, i * j * k);
                    }
                }
            }
            
            Grid1D result = grid1 + grid2;
        }
        auto end = chrono::high_resolution_clock::now();
        auto duration1d = chrono::duration_cast<chrono::microseconds>(end - start);
        
        cout << "Grid1D time: " << duration1d.count() / num_tests << " microseconds" << endl;
    }
}

int main() {
    try {
        test_grid1d();
        test_gridvec();
        test_gridnew();
        performance_test();
        
        cout << "All tests completed successfully!" << endl;
    } catch (const exception& e) {
        cout << "Test failed with exception: " << e.what() << endl;
        return 1;
    }
    
    return 0;
}
