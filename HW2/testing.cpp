#include <iostream>
#include <string>
#include <vector>
#include <cmath>
#include <sstream>
#include <cassert>

// Not a best practice
#include "homework2_skeleton.cpp"

using namespace std;

// Leave this function unchanged, but call it from main <<<<<
void test_operators()
{
    cout << "Inside test operators" << endl;

    Vector v1(1.0, 2.0);
    Vector v2(3.0, 4.0);
    cout << "v2: " << v2 << endl;
    cout << "v1: " << v1 << endl;
    cout << "3.2 * v1: " << 3.2 * v1 << endl;
    cout << "v1 * 3.2: " << v1 * 3.2 << endl;
    cout << "v1 + v2: " << v1 + v2 << endl;
    double prod;
    prod = (2. * v1 + 3. * v2) * (v1 + v2);
    cout << "( 2.*v1 + 3.*v2 ) * (v1 + v2) = " << prod << endl;
    prod = (2. * v1 + 3. * v2) * v1;
    cout << "( 2.*v1 + 3.*v2 ) * v1 = " << prod << endl;
    prod = v1 * v2;
    cout << "v1 * v2 : " << prod << endl;
    cout << "Vector addition: " << v1 << " + " << v2 << " = " << v1 + v2 << endl;
}

// ----------------------------------------------------------------------

void testAddition() 
{
    // Notice: the vector is 4-D
    // a({...}) is appropriate for Vector(const std::vector<double>& components_)
    // What is happening? 
    // Initialize a vector using {...}
    vector<double> comp{2.,3.,6.,2.,-2.};

// The vector argument of the Vector constructor is initialized with {...}
// It is possible to add to the Vector class so that 
    // Vector a{.2, 2.3, 5.2, -2.} is valid. For that, one uses the concept 
    // of initializers (not yet covered). 
Vector a({.2, 2.3, 5.2, -2.});
Vector b({.3, 3.8, 4.2, 4.});
Vector expected({.5, 6.1, 9.4, 2.});
Vector c = a + b;
    assert(c == expected);
    cout << "==> testAddition(4D) passed" << endl;

    Vector a1({.1, .2, .1, .2, .3, .2, .1});
    Vector b1({.1, .2, .1, .2, .3, .2, .1});
    Vector c1 = a1 + b1;
    Vector expected1({.2, .4, .2, .4, .6, .4, .2});
    assert(c1 == expected1);
    cout << "==> testAddition(7D) passed" << endl;
}

// Fill in the following functions
void testSubtraction()
{
Vector a({1.0, 2.0, 3.0});
Vector b({0.5, 1.5, -1.0});
Vector expected({0.5, 0.5, 4.0});
Vector result = a + (-1.0 * b);
assert(result == expected);
cout << "==> testSubtraction passed" << endl;
}

void testSubtractionOperator()
{
Vector a({1.0, 2.0, 3.0});
Vector b({0.5, 1.5, -1.0});
Vector expected({0.5, 0.5, 4.0});
Vector result = a - b;
assert(result == expected);
cout << "==> testSubtractionOperator passed" << endl;
}

void testMultByDouble()
{
Vector a({-2.0, 4.0, -1.0, 0.5});
Vector expected({-4.0, 8.0, -2.0, 1.0});
Vector result = a * 2.0;
assert(result == expected);
Vector result2 = 2.0 * a;
assert(result2 == expected);
cout << "==> testMultByDouble passed" << endl;
}

void testScalarProduct()
{
Vector a({1.0, 2.0, 3.0});
Vector b({2.0, 3.0, 4.0});
Vector expected({2.0, 6.0, 12.0});
Vector result = a ^ b;
assert(result == expected);
cout << "==> testScalarProduct passed" << endl;
}

void testNorms()
{
Vector v({3.0, 4.0, 0.0});
assert(abs(v.norm(v, "L1") - 7.0) < 1e-9);
assert(abs(v.norm(v, "L2") - 5.0) < 1e-9);
assert(abs(v.norm(v, "Linf") - 4.0) < 1e-9);
cout << "==> testNorms passed" << endl;
}

void testEqualityVector()
{
   // test == operator for Vector 
Vector a({1.0, 1.0});
Vector b({1.0 + 1e-10, 1.0 - 1e-10});
b.setTolerance(1e-9);
a.setTolerance(1e-9);
assert(a == b);
Vector c({1.0, 1.1});
assert(a != c);
cout << "==> testEqualityVector passed" << endl;
}

void testEqualityParticle()
{
   // test == operator for Particle
Particle p1(1.0, Vector(0.0, 0.0), Vector(1.0, 0.0), Vector(0.0, 0.0));
Particle p2(1.0, Vector(0.0, 0.0), Vector(1.0, 0.0), Vector(0.0, 0.0));
assert(p1 == p2);
Particle p3(1.1, Vector(0.0, 0.0), Vector(1.0, 0.0), Vector(0.0, 0.0));
assert(p1 != p3);
cout << "==> testEqualityParticle passed" << endl;
}

void testCoutVector()
{
Vector v({1.0, 2.0, 3.0});
std::stringstream ss;
ss << v;
assert(ss.str() == string("(1, 2, 3)"));
cout << "==> testCoutVector passed" << endl;
}

void testCoutParticle()
{
Particle p(2.0, Vector(0.0, 1.0), Vector(1.0, 0.0), Vector(0.0, 0.0));
std::stringstream ss;
ss << p;
// Basic sanity check that key fields are present
auto s = ss.str();
assert(s.find("Particle(mass=") != string::npos);
assert(s.find("position=") != string::npos);
assert(s.find("velocity=") != string::npos);
cout << "==> testCoutParticle passed" << endl;
}

void testParticleSimulation()
{
Particle p(1.0, Vector(0.0, 0.0), Vector(1.0, 0.0), Vector(0.0, 0.0));
Vector initial_pos = p.position_;
Vector initial_vel = p.velocity_;

// Update particle
p.update(0.0, 0.1);

// Check that position and velocity changed
assert(p.position_ != initial_pos);
assert(p.velocity_ != initial_vel);
cout << "==> testParticleSimulation passed" << endl;
}

void testErrorHandling()
{
Vector a({1.0, 2.0});
Vector b({1.0, 2.0, 3.0});
bool exception_thrown = false;
try {
Vector c = a + b;
} catch (const std::invalid_argument& e) {
exception_thrown = true;
}
assert(exception_thrown);
cout << "==> testErrorHandling passed" << endl;
}

void testDotProduct()
{
Vector a({1.0, 2.0, 3.0});
Vector b({4.0, 5.0, 6.0});
double expected = 1.0*4.0 + 2.0*5.0 + 3.0*6.0; // 32.0
double result = a * b;
assert(abs(result - expected) < 1e-9);
cout << "==> testDotProduct passed" << endl;
}

void testMultiDimensionalVectors()
{
// Test 2D
Vector v2d(1.0, 2.0);
assert(v2d.size() == 2);

// Test 3D
Vector v3d(1.0, 2.0, 3.0);
assert(v3d.size() == 3);

// Test 6D
Vector v6d({1.0, 2.0, 3.0, 4.0, 5.0, 6.0});
assert(v6d.size() == 6);

cout << "==> testMultiDimensionalVectors passed" << endl;
}

//----------------------------------------------------------------------
int main()
{
    // Leave as is
    test_operators();
testAddition();
testSubtraction();
testSubtractionOperator();
testMultByDouble();
testScalarProduct();
testNorms();
testEqualityVector();
testEqualityParticle();
testCoutVector();
testCoutParticle();
testParticleSimulation();
testErrorHandling();
testDotProduct();
testMultiDimensionalVectors();

cout << "\n=== ALL TESTS PASSED ===" << endl;
}
