#ifndef SECANT_H
#define SECANT_H

#include <iostream>
#include <cmath>
#include "function.h"
#include "solver.h"
using namespace std;

/**
 * @brief Secant method implementation for root finding.
 * 
 * The secant method is a root-finding algorithm that uses a succession of roots
 * of secant lines to better approximate a root of a function. It does not require
 * the derivative of the function, making it useful when derivatives are difficult
 * to compute.
 * 
 * Formula: x_{n+1} = x_n - f(x_n) * (x_n - x_{n-1}) / (f(x_n) - f(x_{n-1}))
 * 
 * @tparam T The numeric type (float or double) for calculations
 */
template <typename T>
class Secant : public Solver<T> {
public:
    /**
     * @brief Construct a new Secant object
     * 
     * @param tolerance Tolerance for function value convergence (default: 1.e-3)
     * @param root_tolerance Tolerance for root value convergence (default: 1.e-3)
     * @param maxIterations Maximum number of iterations allowed (default: 5)
     */
    Secant<T>(T tolerance = 1.e-3, T root_tolerance = 1.e-3, int maxIterations = 5);
    
    /**
     * @brief Compute the root of a function using the secant method
     * 
     * Implements the secant method to find the root of the given function.
     * The algorithm starts from the two endpoints of the initial bracket and
     * iteratively refines the estimate using function values at two points.
     * 
     * @param func Reference to the function whose root is to be computed
     * @param bracket_tol Tolerance for bracket refinement (unused in secant method)
     * @return T The computed root value
     */
    T computeRoot(Function<T>& func, T bracket_tol) override;
    
    /**
     * @brief Destructor for Secant solver
     */
    ~Secant() override;
};

template <typename T>
Secant<T>::Secant(T tolerance, T root_tolerance, int maxIterations) 
    : Solver<T>(tolerance, root_tolerance, maxIterations, "Secant") {}

template <typename T>
Secant<T>::~Secant() {
    std::cout << "Secant destructor" << std::endl;
}

template <typename T>
T Secant<T>::computeRoot(Function<T>& func, T bracket_tol) {
    // Get initial bracket
    auto bracket = func.getBracket();
    T x0 = bracket.first;
    T x1 = bracket.second;
    
    T fx0 = func(x0);
    T fx1 = func(x1);
    
    this->finalIteration = 0;
    
    for (int i = 0; i < this->maxIterations; i++) {
        // Check if function values are too close
        if (abs(fx1 - fx0) < 1e-12) {
            std::cout << "Warning: Function values too close at iteration " << i << std::endl;
            break;
        }
        
        // Secant method: x_new = x1 - f(x1) * (x1 - x0) / (f(x1) - f(x0))
        T x2 = x1 - fx1 * (x1 - x0) / (fx1 - fx0);
        
        // Check convergence
        if (abs(fx1) < this->tolerance) {
            func.setRoot(x1);
            this->finalIteration = i + 1;
            return x1;
        }
        
        if (abs(x2 - x1) < this->root_tolerance) {
            func.setRoot(x2);
            this->finalIteration = i + 1;
            return x2;
        }
        
        // Update for next iteration
        x0 = x1;
        x1 = x2;
        fx0 = fx1;
        fx1 = func(x1);
        
        this->finalIteration = i + 1;
    }
    
    func.setRoot(x1);
    return x1;
}

#endif
