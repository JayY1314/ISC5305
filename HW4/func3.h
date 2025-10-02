#ifndef FUNC3_H
#define FUNC3_H

#include <iostream>
#include <cmath>
#include "function.h"
using namespace std;

/**
 * @brief Implementation of f(x) = log(x) + x^2 - 3
 * 
 * This class implements the logarithmic-quadratic function log(x) + x^2 - 3 and its derivative.
 * The function combines logarithmic and quadratic terms, creating a non-linear equation.
 * 
 * @tparam T The numeric type (float or double) for calculations
 */
template <typename T>
class Func3 : public Function<T> {
public:
    /**
     * @brief Construct a new Func3 object
     * 
     * @param x0 Left endpoint of the initial bracket (default: 1.0)
     * @param x1 Right endpoint of the initial bracket (default: 2.0)
     */
    Func3(T x0 = 1., T x1 = 2.);
    
    /**
     * @brief Destructor for Func3
     */
    ~Func3();
    
    /**
     * @brief Evaluate the function at point x
     * 
     * @param x The input value (must be positive for log(x))
     * @return T The function value log(x) + x^2 - 3
     */
    T operator()(T x) override;
    
    /**
     * @brief Evaluate the derivative at point x
     * 
     * @param x The input value (must be positive for 1/x)
     * @return T The derivative value 1/x + 2x
     */
    T fp(T x) override;
};

template <typename T>
Func3<T>::Func3(T x0, T x1) : Function<T>("log(x)+x^2-3", x0, x1) {}

template <typename T>
Func3<T>::~Func3() {
    std::cout << "Func3 destructor" << std::endl;
}

template <typename T>
T Func3<T>::operator()(T x) {
    return log(x) + x * x - 3.0;
}

template <typename T>
T Func3<T>::fp(T x) {
    return 1.0 / x + 2.0 * x;
}

#endif
