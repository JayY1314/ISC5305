#ifndef SOLVER_H
#define SOLVER_H

#include <iostream>
#include <string>
#include <utility>  // for std::pair
#include "function.h"

/**
 * @brief Template base class for root finding algorithms.
 * 
 * This abstract base class provides a common interface for root finding algorithms
 * such as Newton's method and the Secant method. It supports both float and double
 * precision through template specialization.
 * 
 * @tparam T The numeric type (float or double) for calculations
 */
template <typename T>
class Solver {
protected:
    int finalIteration;          ///< Number of iterations performed in the last computation
    int maxIterations;           ///< Maximum number of iterations allowed
    std::string name;            ///< Name of the solver algorithm
    T tolerance;                 ///< Tolerance for function value convergence
    T root_tolerance;            ///< Tolerance for root value convergence
    T bracket_tol;               ///< Tolerance for bracket refinement
    std::pair<T, T> bracket;     ///< Current bracket interval

public:
    /**
     * @brief Construct a new Solver object
     * 
     * @param tolerance_ Tolerance for function value convergence
     * @param root_tolerance_ Tolerance for root value convergence
     * @param maxIterations_ Maximum number of iterations allowed
     * @param name_ Name of the solver algorithm
     */
    Solver(T tolerance_, T root_tolerance_, int maxIterations_, const std::string name_);
    
    /**
     * @brief Virtual destructor to ensure proper cleanup of derived classes
     */
    virtual ~Solver();

    /**
     * @brief Pure virtual function for computing the root of a function
     * 
     * Must be implemented by derived classes to define the specific root finding algorithm.
     * 
     * @param func Reference to the function whose root is to be computed
     * @param bracket_tol Tolerance for bracket refinement
     * @return T The computed root value
     */
    virtual T computeRoot(Function<T>& func, T bracket_tol) = 0;

    /**
     * @brief Get the number of iterations performed in the last computation
     * 
     * @return int Number of iterations performed
     */
    int getFinalIteration();
    
    /**
     * @brief Get the maximum number of iterations allowed
     * 
     * @return int Maximum number of iterations
     */
    int getMaxIterations();
    
    /**
     * @brief Set the maximum number of iterations allowed
     * 
     * @param maxIters New maximum number of iterations
     */
    void setMaxIterations(int maxIters);
    
    /**
     * @brief Get the name of the solver algorithm
     * 
     * @return std::string Name of the solver
     */
    std::string getName();
    
    /**
     * @brief Refine the bracket interval using bisection method
     * 
     * Uses recursive bisection to narrow down the bracket interval containing the root.
     * This improves convergence of the root finding algorithm.
     * 
     * @param func Reference to the function
     * @param bracket The bracket interval to refine
     * @param bracket_tol Tolerance for bracket refinement
     */
    void refineBracket(Function<T>& func, std::pair<T, T> bracket, T bracket_tol);
    
    /**
     * @brief Get the current bracket interval
     * 
     * @return std::pair<T, T> Current bracket interval
     */
    std::pair<T, T> getBracket();
    
    /**
     * @brief Get the bracket tolerance
     * 
     * @return T Current bracket tolerance
     */
    T getBracketTolerance();
    
    /**
     * @brief Set the bracket tolerance
     * 
     * @param bracket_tol New bracket tolerance
     */
    void setBracketTolerance(T bracket_tol);
};

// Implementation of Solver class methods
template <typename T>
Solver<T>::Solver(T tolerance_, T root_tolerance_, int maxIterations_, const std::string name_)
    : finalIteration(0), maxIterations(maxIterations_), name(name_), 
      tolerance(tolerance_), root_tolerance(root_tolerance_), bracket_tol(0.0) {}

template <typename T>
Solver<T>::~Solver() {}

template <typename T>
int Solver<T>::getFinalIteration() {
    return finalIteration;
}

template <typename T>
int Solver<T>::getMaxIterations() {
    return maxIterations;
}

template <typename T>
void Solver<T>::setMaxIterations(int maxIters) {
    maxIterations = maxIters;
}

template <typename T>
std::string Solver<T>::getName() {
    return name;
}

template <typename T>
void Solver<T>::refineBracket(Function<T>& func, std::pair<T, T> bracket_, T bracket_tol) {
    this->bracket = bracket_;
    this->bracket_tol = bracket_tol;

    // Call recursive bisection method
    this->bracket = bisectionRecursive(func, bracket_, bracket_tol);
}

template <typename T>
std::pair<T, T> Solver<T>::getBracket() {
    return bracket;
}

template <typename T>
T Solver<T>::getBracketTolerance() {
    return bracket_tol;
}

template <typename T>
void Solver<T>::setBracketTolerance(T bracket_tol) {
    this->bracket_tol = bracket_tol;
}

// Forward declaration for bisectionRecursive
template <typename T>
std::pair<T, T> bisectionRecursive(Function<T>& func, std::pair<T, T> bracket, T bracket_tol);

/**
 * @brief Recursive implementation of bisection method for bracket refinement.
 * 
 * This function recursively narrows down the bracket interval containing the root
 * by repeatedly bisecting the interval and checking for sign changes.
 * 
 * @param func The function for which the root is being computed
 * @param bracket The current bracket interval
 * @param bracket_tol The tolerance for the bracket interval
 * @return The refined bracket interval
 */
template <typename T>
std::pair<T, T> bisectionRecursive(Function<T>& func, std::pair<T, T> bracket, T bracket_tol) {
    T center = (bracket.first + bracket.second) / 2.0;
    T left_value = func(bracket.first);
    T right_value = func(bracket.second);
    T center_value = func(center);

    // Base case: function value at center is within tolerance
    if (std::abs(center-bracket.first) <= bracket_tol) {
        return bracket;
    }

    // Recursive case: determine which half contains the root
    std::pair<T, T> new_bracket;

    if (left_value * center_value < 0) {
        // Root is in left half
        new_bracket = {bracket.first, center};
    } else if (right_value * center_value < 0) {
        // Root is in right half
        new_bracket = {center, bracket.second};
    } else {
        // No sign change, try to narrow by moving both endpoints toward center
        T new_left = bracket.first + (center - bracket.first) / 2.0;
        T new_right = center + (bracket.second - center) / 2.0;
        new_bracket = {new_left, new_right};
    }

    // Recursive call with the new bracket
    return bisectionRecursive(func, new_bracket, bracket_tol);
}

#endif
