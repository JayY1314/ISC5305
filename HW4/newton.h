#ifndef NEWTON_H
#define NEWTON_H

#include <iostream>
#include <cmath>
#include "function.h"
#include "solver.h"
using namespace std;

/**
 * @brief Newton's method implementation for root finding.
 * 
 * Newton's method (also known as Newton-Raphson method) is an iterative algorithm
 * for finding the roots of a differentiable function. It uses the function value
 * and its derivative to converge to the root.
 * 
 * Formula: x_{n+1} = x_n - f(x_n) / f'(x_n)
 * 
 * @tparam T The numeric type (float or double) for calculations
 */
template <typename T>
class Newton : public Solver<T> {
public:
    /**
     * @brief Construct a new Newton object
     * 
     * @param tolerance Tolerance for function value convergence (default: 1.e-3)
     * @param root_tolerance Tolerance for root value convergence (default: 1.e-3)
     * @param maxIterations Maximum number of iterations allowed (default: 5)
     */
    Newton<T>(T tolerance = 1.e-3, T root_tolerance = 1.e-3, int maxIterations = 5);
    
    /**
     * @brief Compute the root of a function using Newton's method
     * 
     * Implements Newton's method to find the root of the given function.
     * The algorithm starts from the midpoint of the initial bracket and
     * iteratively refines the estimate using the function value and derivative.
     * 
     * @param func Reference to the function whose root is to be computed
     * @param bracket_tol Tolerance for bracket refinement (unused in Newton's method)
     * @return T The computed root value
     */
    T computeRoot(Function<T>& func, T bracket_tol) override;
    
    /**
     * @brief Destructor for Newton solver
     */
    ~Newton() override;
};

template <typename T>
Newton<T>::Newton(T tolerance, T root_tolerance, int maxIterations) 
    : Solver<T>(tolerance, root_tolerance, maxIterations, "Newton") {}

template <typename T>
Newton<T>::~Newton() {
    std::cout << "Newton destructor" << std::endl;
}

template <typename T>
T Newton<T>::computeRoot(Function<T>& func, T bracket_tol) {
    // Get initial bracket
    auto bracket = func.getBracket();
    T x0 = bracket.first;
    T x1 = bracket.second;
    
    // Use midpoint as initial guess
    T x = (x0 + x1) / 2.0;
    T prev_x = x;
    
    this->finalIteration = 0;
    
    for (int i = 0; i < this->maxIterations; i++) {
        T fx = func(x);
        T fpx = func.fp(x);
        
        // Check if derivative is too small
        if (abs(fpx) < 1e-12) {
            std::cout << "Warning: Derivative too small at iteration " << i << std::endl;
            break;
        }
        
        // Newton's method: x_new = x - f(x)/f'(x)
        prev_x = x;
        x = x - fx / fpx;
        
        this->finalIteration = i + 1;
        
        // Check convergence
        if (abs(fx) < this->tolerance) {
            break;
        }
        
        if (abs(x - prev_x) < this->root_tolerance) {
            break;
        }
    }
    
    func.setRoot(x);
    return x;
}

#endif
