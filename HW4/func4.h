#ifndef FUNC4_H
#define FUNC4_H

#include <iostream>
#include <cmath>
#include "function.h"
using namespace std;

/**
 * @brief Implementation of f(x) = x^2 - 3
 * 
 * This class implements the simple quadratic function x^2 - 3 and its derivative.
 * The function has roots at x = 3  1.732.
 * 
 * @tparam T The numeric type (float or double) for calculations
 */
template <typename T>
class Func4 : public Function<T> {
public:
    /**
     * @brief Construct a new Func4 object
     * 
     * @param x0 Left endpoint of the initial bracket (default: 0.0)
     * @param x1 Right endpoint of the initial bracket (default: 2.0)
     */
    Func4(T x0 = 0., T x1 = 2.);
    
    /**
     * @brief Destructor for Func4
     */
    ~Func4();
    
    /**
     * @brief Evaluate the function at point x
     * 
     * @param x The input value
     * @return T The function value x^2 - 3
     */
    T operator()(T x) override;
    
    /**
     * @brief Evaluate the derivative at point x
     * 
     * @param x The input value
     * @return T The derivative value 2x
     */
    T fp(T x) override;
};

template <typename T>
Func4<T>::Func4(T x0, T x1) : Function<T>("x^2-3", x0, x1) {}

template <typename T>
Func4<T>::~Func4() {
    std::cout << "Func4 destructor" << std::endl;
}

template <typename T>
T Func4<T>::operator()(T x) {
    return x * x - 3.0;
}

template <typename T>
T Func4<T>::fp(T x) {
    return 2.0 * x;
}

#endif
