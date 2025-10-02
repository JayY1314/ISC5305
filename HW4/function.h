#ifndef FUNCTION_H
#define FUNCTION_H

// Add header files as needed
#include <cmath>
#include <string>
#include <utility>
using namespace std;

/**
 * @brief Template base class for mathematical functions used in root finding algorithms.
 * 
 * This abstract base class provides a common interface for mathematical functions
 * that can be used with root finding algorithms. It supports both float and double
 * precision through template specialization.
 * 
 * @tparam T The numeric type (float or double) for function evaluation
 */
template <typename T>
class Function {
protected:
    std::string name;                    ///< Human-readable name of the function
    std::pair<T, T> bracket;            ///< Initial bracket [x0, x1] containing the root
    T root;                             ///< Computed root value

public:
    /**
     * @brief Construct a new Function object
     * 
     * @param name_ Human-readable name of the function
     * @param x0 Left endpoint of the initial bracket
     * @param x1 Right endpoint of the initial bracket
     */
    Function(const std::string name_, T x0 = 0., T x1 = 1.);
    
    /**
     * @brief Virtual destructor to ensure proper cleanup of derived classes
     */
    virtual ~Function();

    /**
     * @brief Pure virtual function for function evaluation
     * 
     * Must be implemented by derived classes to define the actual mathematical function.
     * 
     * @param x The input value at which to evaluate the function
     * @return T The function value at x
     */
    virtual T operator()(const T x) = 0;
    
    /**
     * @brief Pure virtual function for derivative evaluation
     * 
     * Must be implemented by derived classes to define the derivative of the function.
     * Used by Newton's method for root finding.
     * 
     * @param x The input value at which to evaluate the derivative
     * @return T The derivative value at x
     */
    virtual T fp(T x) = 0;

    /**
     * @brief Get the name of the function
     * 
     * @return std::string The human-readable name of the function
     */
    std::string getName();
    
    /**
     * @brief Get the residual (absolute value of function at computed root)
     * 
     * @return T The absolute value of the function evaluated at the computed root
     */
    T getResidual();
    
    /**
     * @brief Get the computed root value
     * 
     * @return T The computed root value
     */
    T getRoot();
    
    /**
     * @brief Set the computed root value
     * 
     * @param root_ The computed root value to set
     */
    void setRoot(T root_);

    /**
     * @brief Verify the accuracy of a computed root
     * 
     * Evaluates the function at the given value and returns the absolute error.
     * This is used to verify that the computed root is actually a root of the function.
     * 
     * @param x The value to verify as a potential root
     * @return T The absolute value of the function at x (should be close to zero for a good root)
     */
    T verify(T x);

    /**
     * @brief Get the current bracket interval
     * 
     * @return std::pair<T, T> The bracket interval [x0, x1]
     */
    std::pair<T, T> getBracket();
    
    /**
     * @brief Set the bracket interval
     * 
     * @param x0 Left endpoint of the bracket
     * @param x1 Right endpoint of the bracket
     */
    void setBracket(T x0, T x1);
};

// Implementation of Function class methods
template <typename T>
Function<T>::Function(const std::string name_, T x0, T x1) 
    : name(name_), bracket(x0, x1), root(0.0) {}

template <typename T>
Function<T>::~Function() {}

template <typename T>
std::string Function<T>::getName() {
    return name;
}

template <typename T>
T Function<T>::getResidual() {
    return abs((*this)(root));
}

template <typename T>
T Function<T>::getRoot() {
    return root;
}

template <typename T>
void Function<T>::setRoot(T root_) {
    root = root_;
}

template <typename T>
T Function<T>::verify(T x) {
    return abs((*this)(x));
}

template <typename T>
std::pair<T, T> Function<T>::getBracket() {
    return bracket;
}

template <typename T>
void Function<T>::setBracket(T x0, T x1) {
    bracket = std::make_pair(x0, x1);
}

#endif
