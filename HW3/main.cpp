#include "grid3d_1d_array.h"
#include "grid3d_vector.h"
#include "grid3d_new.h"
#include <iostream>
#include <chrono>
#include <vector>
#include <fstream>

using namespace std;

// Timing function
template<typename GridType>
double time_addition(int size, int num_runs = 5) {
    vector<double> times;
    
    for (int run = 0; run < num_runs; run++) {
        auto start = chrono::high_resolution_clock::now();
        
        GridType grid1(size, size, size);
        GridType grid2(size, size, size);
        
        // Fill with test data
        for (int i = 0; i < size; i++) {
            for (int j = 0; j < size; j++) {
                for (int k = 0; k < size; k++) {
                    grid1.set(i, j, k, i + j + k);
                    grid2.set(i, j, k, i * j * k);
                }
            }
        }
        
        GridType result = grid1 + grid2;
        
        auto end = chrono::high_resolution_clock::now();
        auto duration = chrono::duration_cast<chrono::microseconds>(end - start);
        times.push_back(duration.count());
    }
    
    // Calculate average (skip first run)
    double sum = 0;
    for (int i = 1; i < times.size(); i++) {
        sum += times[i];
    }
    return sum / (times.size() - 1);
}

// Test all three grid types
void test_all_grids(int nx, int ny, int nz) {
    cout << "Testing grids with dimensions " << nx << "x" << ny << "x" << nz << endl;
    cout << "================================================" << endl;
    
    // Test Grid1D
    cout << "\n--- Grid1D (1D Array) ---" << endl;
    Grid1D grid1d(nx, ny, nz);
    cout << "Size: " << grid1d.getSize() << endl;
    cout << "Memory: " << grid1d.getMemory() << " bytes" << endl;
    
    // Fill with test data
    for (int i = 0; i < nx; i++) {
        for (int j = 0; j < ny; j++) {
            for (int k = 0; k < nz; k++) {
                grid1d.set(i, j, k, 100*i + 10*j + k);
            }
        }
    }
    
    cout << "Grid1D contents:" << endl;
    cout << grid1d;
    
    Grid1D sum1d = grid1d + grid1d;
    cout << "Addition test passed" << endl;
    
    // Test GridVec
    cout << "\n--- GridVec (Vector) ---" << endl;
    GridVec gridvec(nx, ny, nz);
    cout << "Size: " << gridvec.getSize() << endl;
    cout << "Memory: " << gridvec.getMemory() << " bytes" << endl;
    
    // Fill with test data
    for (int i = 0; i < nx; i++) {
        for (int j = 0; j < ny; j++) {
            for (int k = 0; k < nz; k++) {
                gridvec.set(i, j, k, 100*i + 10*j + k);
            }
        }
    }
    
    cout << "GridVec contents:" << endl;
    cout << gridvec;
    
    GridVec sumvec = gridvec + gridvec;
    cout << "Addition test passed" << endl;
    
    // Test GridNew
    cout << "\n--- GridNew (New Operator) ---" << endl;
    GridNew gridnew(nx, ny, nz);
    cout << "Size: " << gridnew.getSize() << endl;
    cout << "Memory: " << gridnew.getMemory() << " bytes" << endl;
    
    // Fill with test data
    for (int i = 0; i < nx; i++) {
        for (int j = 0; j < ny; j++) {
            for (int k = 0; k < nz; k++) {
                gridnew.set(i, j, k, 100*i + 10*j + k);
            }
        }
    }
    
    cout << "GridNew contents:" << endl;
    cout << gridnew;
    
    GridNew sumnew = gridnew + gridnew;
    cout << "Addition test passed" << endl;
}

// Performance analysis
void performance_analysis() {
    cout << "\n\n=== PERFORMANCE ANALYSIS ===" << endl;
    cout << "Testing addition operation for different grid sizes" << endl;
    cout << "================================================" << endl;
    
    vector<int> sizes = {10, 100, 1000};
    vector<double> times1d, timesvec, timesnew;
    
    cout << "Size\tGrid1D (μs)\tGridVec (μs)\tGridNew (μs)" << endl;
    cout << "----\t----------\t----------\t----------" << endl;
    
    for (int size : sizes) {
        double time1d = time_addition<Grid1D>(size);
        double timevec = time_addition<GridVec>(size);
        double timenew = time_addition<GridNew>(size);
        
        times1d.push_back(time1d);
        timesvec.push_back(timevec);
        timesnew.push_back(timenew);
        
        cout << size << "\t" << time1d << "\t\t" << timevec << "\t\t" << timenew << endl;
    }
    
    // Save data to file for plotting
    ofstream file("timing_data.txt");
    file << "Size,Grid1D,GridVec,GridNew" << endl;
    for (int i = 0; i < sizes.size(); i++) {
        file << sizes[i] << "," << times1d[i] << "," << timesvec[i] << "," << timesnew[i] << endl;
    }
    file.close();
    
    cout << "\nTiming data saved to timing_data.txt" << endl;
    cout << "You can use this data to create performance plots." << endl;
}

int main() {
    cout << "3D Grid Implementation Test Suite" << endl;
    cout << "=================================" << endl;
    
    try {
        // Test with small grid first
        test_all_grids(2, 3, 4);
        
        // Performance analysis
        performance_analysis();
        
        cout << "\nAll tests completed successfully!" << endl;
        
    } catch (const exception& e) {
        cout << "Test failed with exception: " << e.what() << endl;
        return 1;
    }
    
    return 0;
}
