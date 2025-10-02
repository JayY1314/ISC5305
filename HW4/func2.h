#ifndef FUNC2_H
#define FUNC2_H

#include <iostream>
#include <cmath>
#include "function.h"

/**
 * @brief Implementation of f(x) = x^3 - 6x^2 + 11x - 4
 * 
 * This class implements the cubic polynomial x^3 - 6x^2 + 11x - 4 and its derivative.
 * The function can be factored as (x-1)(x-2)(x-2), so it has roots at x=1 and x=2 (double root).
 * 
 * @tparam T The numeric type (float or double) for calculations
 */
template <typename T>
class Func2 : public Function<T> {
public:
    /**
     * @brief Construct a new Func2 object
     * 
     * @param x0 Left endpoint of the initial bracket (default: 0.0)
     * @param x1 Right endpoint of the initial bracket (default: 1.0)
     */
    Func2(T x0 = 0., T x1 = 1.);
    
    /**
     * @brief Destructor for Func2
     */
    ~Func2();
    
    /**
     * @brief Evaluate the function at point x
     * 
     * @param x The input value
     * @return T The function value x^3 - 6x^2 + 11x - 4
     */
    T operator()(T x) override;
    
    /**
     * @brief Evaluate the derivative at point x
     * 
     * @param x The input value
     * @return T The derivative value 3x^2 - 12x + 11
     */
    T fp(T x) override;
};

template <typename T>
Func2<T>::Func2(T x0, T x1) : Function<T>("x^3-6x^2+11x-4", x0, x1) {}

template <typename T>
Func2<T>::~Func2() {
    std::cout << "Func2 destructor" << std::endl;
}

template <typename T>
T Func2<T>::operator()(T x) {
    return x * x * x - 6 * x * x + 11 * x - 4.0;
}

template <typename T>
T Func2<T>::fp(T x) {
    return 3 * x * x - 12 * x + 11;
}

#endif
