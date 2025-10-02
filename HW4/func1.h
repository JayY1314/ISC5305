#ifndef FUNC1_H
#define FUNC1_H

#include <iostream>
#include <cmath>
#include "function.h"

/**
 * @brief Implementation of f(x) = sin(3x - 2)
 * 
 * This class implements the trigonometric function sin(3x - 2) and its derivative.
 * The function has roots where 3x - 2 = n*pi, so x = (n*pi + 2)/3 for integer n.
 * 
 * @tparam T The numeric type (float or double) for calculations
 */
template <typename T>
class Func1 : public Function<T> {
public:
    /**
     * @brief Construct a new Func1 object
     * 
     * @param x0 Left endpoint of the initial bracket (default: 1.5)
     * @param x1 Right endpoint of the initial bracket (default: 1.9)
     */
    Func1(T x0 = 1.5, T x1 = 1.9);
    
    /**
     * @brief Evaluate the function at point x
     * 
     * @param x The input value
     * @return T The function value sin(3x - 2)
     */
    T operator()(T x) override;
    
    /**
     * @brief Evaluate the derivative at point x
     * 
     * @param x The input value
     * @return T The derivative value 3*cos(3x - 2)
     */
    T fp(T x) override;
    
    /**
     * @brief Destructor for Func1
     */
    ~Func1();
};

template <typename T>
Func1<T>::Func1(T x0, T x1) : Function<T>("sin(3x-2)", x0, x1) {}

template <typename T>
Func1<T>::~Func1() {
    std::cout << "Func1 destructor" << std::endl;
}

template <typename T>
T Func1<T>::operator()(T x) {
    return sin(3.0 * x - 2.0);
}

template <typename T>
T Func1<T>::fp(T x) {
    return 3.0 * cos(3.0 * x - 2.0);
}

#endif
