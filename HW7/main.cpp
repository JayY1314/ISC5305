#include "PolynomialBinaryTree.h"
#include <iostream>
#include <fstream>
#include <vector>
#include <map>
#include <iomanip>
#include <stdexcept>
#include <algorithm>

PolynomialBinaryTree from_zeros(const std::vector<double>& zeros) {
    if (zeros.empty()) {
        throw std::invalid_argument("Zero vector cannot be empty");
    }

    std::map<double, int> zeroCounts;
    for (double z : zeros) {
        zeroCounts[z]++;
    }

    PolynomialBinaryTree result(1.0);
    
    for (const auto& pair : zeroCounts) {
        double zero = pair.first;
        int multiplicity = pair.second;
        
        PolynomialBinaryTree factor = PolynomialBinaryTree("x") - PolynomialBinaryTree(zero);
        
        for (int i = 0; i < multiplicity; ++i) {
            result = result * factor;
        }
    }
    
    return result;
}

int main() {
    try {
        std::vector<double> zeros = {-0.86, -0.34, 0.34, 0.86};
        
        PolynomialBinaryTree poly = from_zeros(zeros);
        
        std::ofstream outFile("polynomial_evaluation.txt");
        if (!outFile.is_open()) {
            std::cerr << "Error: Cannot open polynomial_evaluation.txt for writing" << std::endl;
            return 1;
        }

        outFile << std::fixed << std::setprecision(4);
        outFile << "x\tP(x)\tP'(x)\tP''(x)\tP'''(x)\tP''''(x)\tP'''''(x)\n";

        int numPoints = 100;
        double start = -1.0;
        double end = 1.0;
        double step = (end - start) / (numPoints - 1);

        for (int i = 0; i < numPoints; ++i) {
            double x = start + i * step;
            
            PolynomialBinaryTree p = poly;
            double p0 = p.evaluate(x);
            
            PolynomialBinaryTree p1 = p.differentiate();
            double p1_val = p1.evaluate(x);
            
            PolynomialBinaryTree p2 = p1.differentiate();
            double p2_val = p2.evaluate(x);
            
            PolynomialBinaryTree p3 = p2.differentiate();
            double p3_val = p3.evaluate(x);
            
            PolynomialBinaryTree p4 = p3.differentiate();
            double p4_val = p4.evaluate(x);
            
            PolynomialBinaryTree p5 = p4.differentiate();
            double p5_val = p5.evaluate(x);

            outFile << x << "\t" << p0 << "\t" << p1_val << "\t" 
                    << p2_val << "\t" << p3_val << "\t" << p4_val << "\t" << p5_val << "\n";
        }

        outFile.close();
        std::cout << "Polynomial evaluation completed. Results saved to polynomial_evaluation.txt" << std::endl;

    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    }

    return 0;
}

