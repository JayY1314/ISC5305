#include "brain_mesh_macros.h"

// for sqrt
#include <cmath>
#include <set>
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <array>
#include <string>
#include <iomanip>
#include <memory>
#include <algorithm>
// do not use "using namespace" in include files. 
// use them in implementation files

// T can be `float` or `double` 
// U can be `int` or `long` for the index
BM_TEMPLATE
class BrainMesh
{
private:
	Vertices<T> vertices; 
	Triangles<INT> triangles;
    // triangle areas (or vertex areas)
	std::vector<T> triangleAreas;
	std::vector<T> vertexAreas;
	std::vector<T> edgeLengths;
    // total surface area
	T totalArea;
	int nbPoints; 
	int nbTriangles;
	int nbVertices;
    int nbEdges;
	std::string name;

public:
	// Constructor
	BrainMesh(const std::string& name);

	// Copy Constructor
	BrainMesh(const BrainMesh& other);

	// Copy Assignment Operator
	BrainMesh& operator=(const BrainMesh& other);

	// Move Constructor
	BrainMesh(BrainMesh&& other) noexcept;

	// Destructor
	~BrainMesh() = default;  

	/**
	 * Reads vertex and triangle data from a VTK file
	 * @param fileName Path to the VTK file
	 */
	void readData(const std::string& fileName);

	/**
	 * Calculates the area of a triangle using cross product
	 * @param triangle Triangle indices
	 * @param r12 Vector from vertex 1 to vertex 2
	 * @param r13 Vector from vertex 1 to vertex 3
	 * @param cross Cross product of r12 and r13
	 * @return Area of the triangle
	 */
	T getTriangleArea(const Triangle<INT>& triangle,  
        std::array<T, 3>& r12, std::array<T, 3>& r13, std::array<T, 3>& cross);

	/**
	 * Calculates the area of a triangle using its three vertices
	 * @param triangle Triangle indices
	 * @return Area of the triangle
	 */
	T triangleArea(const Triangle<INT>& triangle);

	/**
	 * Gets reference to triangle areas vector
	 * @return Reference to triangleAreas vector
	 */
    std::vector<T>& getTriangleAreas();

	/**
	 * Gets reference to edge lengths vector
	 * @return Reference to edgeLengths vector
	 */
    std::vector<T>& getEdgeLengths();

	/**
	 * Calculates and returns the total surface area
	 * @return Total surface area of the mesh
	 */
    T getTotalArea();

	/**
	 * Computes areas associated with each vertex
	 * Each vertex gets 1/3 of the area of triangles it belongs to
	 */
	void computeVertexAreas();

	/**
	 * Gets reference to vertex areas vector
	 * @return Reference to vertexAreas vector
	 */
    std::vector<T> getVertexAreas();

	/**
	 * Saves vertex areas to a file
	 * @param fileName Output file name
	 */
	void saveVertexAreas(const std::string& fileName);

	/**
	 * Saves triangle areas to a file
	 * @param fileName Output file name
	 */
	void saveTriangleAreas(const std::string& fileName);

	/**
	 * Saves edge lengths to a file
	 * @param fileName Output file name
	 */
	void saveEdgeLengths(const std::string& fileName);

	/**
	 * Computes all triangle areas and stores them
	 */
	void computeTriangleAreas();

	/**
	 * Computes all edge lengths and stores them
	 */
	void computeEdgeLengths();

	/**
	 * Gets the number of vertices
	 * @return Number of vertices
	 */
	int getNumVertices() const { return nbVertices; }

	/**
	 * Gets the number of triangles
	 * @return Number of triangles
	 */
	int getNumTriangles() const { return nbTriangles; }

	/**
	 * Gets the number of edges
	 * @return Number of edges
	 */
	int getNumEdges() const { return nbEdges; }

	/**
	 * Gets the mesh name
	 * @return Mesh name
	 */
	std::string getName() const { return name; }
};
//----------------------------------------------------------------------
