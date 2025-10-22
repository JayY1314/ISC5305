#include "brain_mesh.h"
#include <iostream>
#include <iomanip>
#include <vector>
#include <memory>

using namespace std;

// Helper function to save vector to file
template<typename T>
void saveVector(const string& fileName, const vector<T>& data) {
    ofstream file(fileName);
    if (!file.is_open()) {
        throw runtime_error("Cannot create file: " + fileName);
    }
    
    for (const auto& value : data) {
        file << setprecision(14) << value << endl;
    }
    
    file.close();
    cout << "Data saved to " << fileName << endl;
}

int main()
{
    bool test_code = false;

    // Set cout precision to 10 decimal places
    // This command sets the precision for floating-point output to 10 digits
    // Example: 3.1415926536 instead of 3.14159
    std::cout << std::setprecision(10) << std::endl;

    if (test_code) {
        // Write a test that demonstrates that your code produces the correct answer. 
        // To do this: create a sample input file (test_file.vtk) that loads vertices
        // and triangles whose areas you know. Apply your `brainMesh' class to this 
        // new file to demonstrate your answer is correct. Also check that the sum of 
        // all the triangle areas is equal to the sum of the areas associated with each vertex.
        
        // Create a simple test case with known triangle areas
        cout << "Running test code..." << endl;
        
        // Create test VTK file
        ofstream testFile("test_file.vtk");
        testFile << "# vtk DataFile Version 2.0\n";
        testFile << "Test Triangle\n";
        testFile << "ASCII\n";
        testFile << "DATASET POLYDATA\n";
        testFile << "POINTS 3 float\n";
        testFile << "0.0 0.0 0.0\n";
        testFile << "1.0 0.0 0.0\n";
        testFile << "0.0 1.0 0.0\n";
        testFile << "POLYGONS 1 4\n";
        testFile << "3 0 1 2\n";
        testFile.close();
        
        // Test with the simple triangle
        auto testBrain = make_unique<BrainMesh<double, int>>("test");
        testBrain->readData("test_file.vtk");
        
        double testTotalArea = testBrain->getTotalArea();
        cout << "Test total area: " << setprecision(14) << testTotalArea << endl;
        cout << "Expected area: 0.5" << endl;
        
        testBrain->computeVertexAreas();
        auto testVertexAreas = testBrain->getVertexAreas();
        double testSumVertexAreas = 0;
        for (auto area : testVertexAreas) {
            testSumVertexAreas += area;
        }
        cout << "Test sum of vertex areas: " << setprecision(14) << testSumVertexAreas << endl;
        cout << "Expected sum: 0.5" << endl;
        
        // Verify that sum of vertex areas equals total area
        if (abs(testSumVertexAreas - testTotalArea) < 1e-10) {
            cout << "Test PASSED: Sum of vertex areas equals total area" << endl;
        } else {
            cout << "Test FAILED: Sum of vertex areas does not equal total area" << endl;
        }
        
        exit(0);
    }

    // Create brain mesh instance using smart pointer
    auto brain = make_unique<BrainMesh<double, long>>("brain");
    brain->readData("Cort_lobe_poly.vtk");

    // Compute total area of the brain
    double totalArea = brain->getTotalArea();
    
    // Output total area with both single and double precision
    cout << "Total brain area (double precision): " << setprecision(14) << totalArea << endl;
    cout << "Total brain area (single precision): " << setprecision(14) << (float)totalArea << endl;

    // Compute vertex areas (each vertex gets 1/3 of the area of triangles it belongs to)
    brain->computeVertexAreas();
    auto vertexAreas = brain->getVertexAreas();
    
    // Save the vertex areas to a file named "vertex_areas.txt"
    brain->saveVertexAreas("vertex_areas.txt");

    // Save the triangle areas to a file named "triangle_areas.txt"
    brain->saveTriangleAreas("triangle_areas.txt");

    // Save the edge lengths to a file named "edge_lengths.txt"
    brain->saveEdgeLengths("edge_lengths.txt");
   
    // Sum the vertex areas
    double sumVertexAreas = 0;
    for (auto area : vertexAreas) {
        sumVertexAreas += area;
    }
    
    cout << "Sum of vertex areas: " << setprecision(14) << sumVertexAreas << endl;   
    cout << "Total brain area: " << setprecision(14) << totalArea << endl;   
    
    // Verify that sum of vertex areas equals total area
    if (abs(sumVertexAreas - totalArea) < 1e-10) {
        cout << "Verification PASSED: Sum of vertex areas equals total area" << endl;
    } else {
        cout << "Verification FAILED: Sum of vertex areas does not equal total area" << endl;
        cout << "Difference: " << setprecision(14) << abs(sumVertexAreas - totalArea) << endl;
    }

    return 0;
}
