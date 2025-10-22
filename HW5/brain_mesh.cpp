#include "brain_mesh.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <cmath>
#include <algorithm>
#include <set>

using namespace std;

// Constructor
BM_TEMPLATE
BM_CLASS::BrainMesh(const string& name) 
    : name(name), totalArea(0), nbPoints(0), nbTriangles(0), nbVertices(0), nbEdges(0)
{
}

// Copy Constructor
BM_TEMPLATE
BM_CLASS::BrainMesh(const BrainMesh& other)
    : vertices(other.vertices),
      triangles(other.triangles),
      triangleAreas(other.triangleAreas),
      vertexAreas(other.vertexAreas),
      edgeLengths(other.edgeLengths),
      totalArea(other.totalArea),
      nbPoints(other.nbPoints),
      nbTriangles(other.nbTriangles),
      nbVertices(other.nbVertices),
      nbEdges(other.nbEdges),
      name(other.name)
{
}

// Copy Assignment Operator
BM_TEMPLATE
BM_CLASS& BM_CLASS::operator=(const BrainMesh& other)
{
    if (this != &other) {
        vertices = other.vertices;
        triangles = other.triangles;
        triangleAreas = other.triangleAreas;
        vertexAreas = other.vertexAreas;
        edgeLengths = other.edgeLengths;
        totalArea = other.totalArea;
        nbPoints = other.nbPoints;
        nbTriangles = other.nbTriangles;
        nbVertices = other.nbVertices;
        nbEdges = other.nbEdges;
        name = other.name;
    }
    return *this;
}

// Move Constructor
BM_TEMPLATE
BM_CLASS::BrainMesh(BrainMesh&& other) noexcept
    : vertices(move(other.vertices)),
      triangles(move(other.triangles)),
      triangleAreas(move(other.triangleAreas)),
      vertexAreas(move(other.vertexAreas)),
      edgeLengths(move(other.edgeLengths)),
      totalArea(other.totalArea),
      nbPoints(other.nbPoints),
      nbTriangles(other.nbTriangles),
      nbVertices(other.nbVertices),
      nbEdges(other.nbEdges),
      name(move(other.name))
{
    // Reset the moved-from object
    other.totalArea = 0;
    other.nbPoints = 0;
    other.nbTriangles = 0;
    other.nbVertices = 0;
    other.nbEdges = 0;
}

// Read data from VTK file
BM_TEMPLATE
void BM_CLASS::readData(const string& fileName)
{
    ifstream file(fileName);
    if (!file.is_open()) {
        throw runtime_error("Cannot open file: " + fileName);
    }

    string line;
    int lineNumber = 0;
    bool readingPoints = false;
    bool readingPolygons = false;
    int pointsRead = 0;
    int polygonsRead = 0;

    while (getline(file, line)) {
        lineNumber++;
        
        // Skip empty lines and comments
        if (line.empty() || line[0] == '#') continue;
        
        // Check for POINTS line
        if (line.find("POINTS") != string::npos) {
            istringstream iss(line);
            string dummy;
            iss >> dummy >> nbPoints >> dummy; // POINTS 191724 float
            vertices.resize(nbPoints);
            readingPoints = true;
            readingPolygons = false;
            continue;
        }
        
        // Check for POLYGONS line
        if (line.find("POLYGONS") != string::npos) {
            istringstream iss(line);
            string dummy;
            iss >> dummy >> nbTriangles >> dummy; // POLYGONS 383241 1533776
            triangles.resize(nbTriangles);
            readingPoints = false;
            readingPolygons = true;
            continue;
        }
        
        // Read vertex data (lines 6 to 191730)
        if (readingPoints && pointsRead < nbPoints) {
            istringstream iss(line);
            T x, y, z;
            if (iss >> x >> y >> z) {
                vertices[pointsRead] = {x, y, z};
                pointsRead++;
            }
        }
        
        // Read polygon data (triangles)
        if (readingPolygons && polygonsRead < nbTriangles) {
            istringstream iss(line);
            int n, v1, v2, v3;
            if (iss >> n >> v1 >> v2 >> v3 && n == 3) {
                triangles[polygonsRead] = {v1, v2, v3};
                polygonsRead++;
            }
        }
    }
    
    file.close();
    
    nbVertices = nbPoints;
    
    // Compute unique edges
    set<pair<INT, INT>> edgeSet;
    for (const auto& triangle : triangles) {
        // Add all three edges of the triangle
        INT v1 = triangle[0], v2 = triangle[1], v3 = triangle[2];
        
        // Ensure edges are ordered (smaller index first)
        if (v1 > v2) swap(v1, v2);
        edgeSet.insert({v1, v2});
        
        if (v2 > v3) swap(v2, v3);
        edgeSet.insert({v2, v3});
        
        if (v1 > v3) swap(v1, v3);
        edgeSet.insert({v1, v3});
    }
    nbEdges = edgeSet.size();
    
    cout << "Loaded " << nbVertices << " vertices and " << nbTriangles << " triangles" << endl;
    cout << "Number of unique edges: " << nbEdges << endl;
}

// Calculate triangle area using cross product
BM_TEMPLATE
T BM_CLASS::getTriangleArea(const Triangle<INT>& triangle, 
                           array<T, 3>& r12, array<T, 3>& r13, array<T, 3>& cross)
{
    // Get the three vertices
    const auto& v1 = vertices[triangle[0]];
    const auto& v2 = vertices[triangle[1]];
    const auto& v3 = vertices[triangle[2]];
    
    // Calculate vectors r12 and r13
    r12[0] = v2[0] - v1[0];
    r12[1] = v2[1] - v1[1];
    r12[2] = v2[2] - v1[2];
    
    r13[0] = v3[0] - v1[0];
    r13[1] = v3[1] - v1[1];
    r13[2] = v3[2] - v1[2];
    
    // Calculate cross product
    cross[0] = r12[1] * r13[2] - r12[2] * r13[1];
    cross[1] = r12[2] * r13[0] - r12[0] * r13[2];
    cross[2] = r12[0] * r13[1] - r12[1] * r13[0];
    
    // Calculate magnitude of cross product
    T magnitude = sqrt(cross[0] * cross[0] + cross[1] * cross[1] + cross[2] * cross[2]);
    
    // Area is half the magnitude
    return magnitude / 2.0;
}

// Calculate triangle area (simplified version)
BM_TEMPLATE
T BM_CLASS::triangleArea(const Triangle<INT>& triangle)
{
    array<T, 3> r12, r13, cross;
    return getTriangleArea(triangle, r12, r13, cross);
}

// Get triangle areas vector
BM_TEMPLATE
vector<T>& BM_CLASS::getTriangleAreas()
{
    return triangleAreas;
}

// Get edge lengths vector
BM_TEMPLATE
vector<T>& BM_CLASS::getEdgeLengths()
{
    return edgeLengths;
}

// Calculate total area
BM_TEMPLATE
T BM_CLASS::getTotalArea()
{
    if (triangleAreas.empty()) {
        computeTriangleAreas();
    }
    
    totalArea = 0;
    for (const auto& area : triangleAreas) {
        totalArea += area;
    }
    
    return totalArea;
}

// Compute vertex areas
BM_TEMPLATE
void BM_CLASS::computeVertexAreas()
{
    if (triangleAreas.empty()) {
        computeTriangleAreas();
    }
    
    vertexAreas.resize(nbVertices, 0);
    
    // For each triangle, add 1/3 of its area to each of its vertices
    for (int i = 0; i < nbTriangles; i++) {
        T area = triangleAreas[i];
        T vertexArea = area / 3.0;
        
        vertexAreas[triangles[i][0]] += vertexArea;
        vertexAreas[triangles[i][1]] += vertexArea;
        vertexAreas[triangles[i][2]] += vertexArea;
    }
}

// Get vertex areas
BM_TEMPLATE
vector<T> BM_CLASS::getVertexAreas()
{
    if (vertexAreas.empty()) {
        computeVertexAreas();
    }
    return vertexAreas;
}

// Save vertex areas to file
BM_TEMPLATE
void BM_CLASS::saveVertexAreas(const string& fileName)
{
    if (vertexAreas.empty()) {
        computeVertexAreas();
    }
    
    ofstream file(fileName);
    if (!file.is_open()) {
        throw runtime_error("Cannot create file: " + fileName);
    }
    
    for (const auto& area : vertexAreas) {
        file << setprecision(14) << area << endl;
    }
    
    file.close();
    cout << "Vertex areas saved to " << fileName << endl;
}

// Save triangle areas to file
BM_TEMPLATE
void BM_CLASS::saveTriangleAreas(const string& fileName)
{
    if (triangleAreas.empty()) {
        computeTriangleAreas();
    }
    
    ofstream file(fileName);
    if (!file.is_open()) {
        throw runtime_error("Cannot create file: " + fileName);
    }
    
    for (const auto& area : triangleAreas) {
        file << setprecision(14) << area << endl;
    }
    
    file.close();
    cout << "Triangle areas saved to " << fileName << endl;
}

// Save edge lengths to file
BM_TEMPLATE
void BM_CLASS::saveEdgeLengths(const string& fileName)
{
    if (edgeLengths.empty()) {
        computeEdgeLengths();
    }
    
    ofstream file(fileName);
    if (!file.is_open()) {
        throw runtime_error("Cannot create file: " + fileName);
    }
    
    for (const auto& length : edgeLengths) {
        file << setprecision(14) << length << endl;
    }
    
    file.close();
    cout << "Edge lengths saved to " << fileName << endl;
}

// Compute all triangle areas
BM_TEMPLATE
void BM_CLASS::computeTriangleAreas()
{
    triangleAreas.resize(nbTriangles);
    
    for (int i = 0; i < nbTriangles; i++) {
        triangleAreas[i] = triangleArea(triangles[i]);
    }
}

// Compute all edge lengths
BM_TEMPLATE
void BM_CLASS::computeEdgeLengths()
{
    set<pair<INT, INT>> edgeSet;
    
    // Collect unique edges
    for (const auto& triangle : triangles) {
        INT v1 = triangle[0], v2 = triangle[1], v3 = triangle[2];
        
        // Add all three edges
        if (v1 > v2) swap(v1, v2);
        edgeSet.insert({v1, v2});
        
        if (v2 > v3) swap(v2, v3);
        edgeSet.insert({v2, v3});
        
        if (v1 > v3) swap(v1, v3);
        edgeSet.insert({v1, v3});
    }
    
    // Calculate lengths
    edgeLengths.clear();
    for (const auto& edge : edgeSet) {
        const auto& v1 = vertices[edge.first];
        const auto& v2 = vertices[edge.second];
        
        T dx = v2[0] - v1[0];
        T dy = v2[1] - v1[1];
        T dz = v2[2] - v1[2];
        
        T length = sqrt(dx * dx + dy * dy + dz * dz);
        edgeLengths.push_back(length);
    }
}
