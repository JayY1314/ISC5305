#include "includes.h"

#include <iostream>
#include <memory>
#include <string>
#include <vector>
#include <fstream>
#include <iomanip>
#include <cmath>

using namespace std;

template <typename T>
class PlotData {
public:
    string func_name;
    string method_name;
    string type_name;
    vector<int> iterations;
    vector<T> roots;
    vector<T> residuals;
};

int main() {
    // Create plot data files
    ofstream newton_plot("newton_plot.csv");
    ofstream secant_plot("secant_plot.csv");
    
    newton_plot << "function,type,iteration,root,residual" << endl;
    secant_plot << "function,type,iteration,root,residual" << endl;
    
    // Different solver parameters
    struct SolverParams {
        float residual_tolerance;
        float root_tolerance;
        int maxIterations;
    };
    vector<SolverParams> params = {
        {1.e-5, 1.e-7, 10},    // floats
        {1e-13, 1.e-14, 15},  // doubles
    };

    // Create vectors for functions and solvers
    vector<unique_ptr<Function<float>>> functions_f;
    vector<unique_ptr<Function<double>>> functions_d;
    vector<unique_ptr<Solver<float>>> solvers_float;
    vector<unique_ptr<Solver<double>>> solvers_double;

    // Create function instances
    functions_f.push_back(make_unique<Func1<float>>(1.5f, 1.9f));
    functions_f.push_back(make_unique<Func2<float>>(0.0f, 1.0f));
    functions_f.push_back(make_unique<Func3<float>>(1.0f, 2.0f));
    functions_f.push_back(make_unique<Func4<float>>(0.0f, 2.0f));

    functions_d.push_back(make_unique<Func1<double>>(1.5, 1.9));
    functions_d.push_back(make_unique<Func2<double>>(0.0, 1.0));
    functions_d.push_back(make_unique<Func3<double>>(1.0, 2.0));
    functions_d.push_back(make_unique<Func4<double>>(0.0, 2.0));

    // Create solver instances
    solvers_float.push_back(make_unique<Newton<float>>(params[0].residual_tolerance, params[0].root_tolerance, params[0].maxIterations));
    solvers_float.push_back(make_unique<Secant<float>>(params[0].residual_tolerance, params[0].root_tolerance, params[0].maxIterations));

    solvers_double.push_back(make_unique<Newton<double>>(params[1].residual_tolerance, params[1].root_tolerance, params[1].maxIterations));
    solvers_double.push_back(make_unique<Secant<double>>(params[1].residual_tolerance, params[1].root_tolerance, params[1].maxIterations));

    cout << "Generating plot data for Newton's method..." << endl;
    
    // Test Newton's method for plotting
    for (const auto& func : functions_d) {
        auto newton_solver = make_unique<Newton<double>>(1e-13, 1e-14, 15);
        
        // Get initial bracket
        auto bracket = func->getBracket();
        double x0 = bracket.first;
        double x1 = bracket.second;
        
        // Use midpoint as initial guess
        double x = (x0 + x1) / 2.0;
        double prev_x = x;
        
        newton_plot << func->getName() << ",double," << 0 << "," << x << "," << abs((*func)(x)) << endl;
        
        for (int i = 0; i < 15; i++) {
            double fx = (*func)(x);
            double fpx = func->fp(x);
            
            // Check if derivative is too small
            if (abs(fpx) < 1e-12) {
                cout << "Warning: Derivative too small for " << func->getName() << " at iteration " << i << endl;
                break;
            }
            
            // Newton's method: x_new = x - f(x)/f'(x)
            prev_x = x;
            x = x - fx / fpx;
            
            newton_plot << func->getName() << ",double," << (i+1) << "," << x << "," << abs((*func)(x)) << endl;
            
            // Check convergence
            if (abs(fx) < 1e-13) {
                cout << func->getName() << " (Newton) converged at iteration " << (i+1) << endl;
                break;
            }
            
            if (abs(x - prev_x) < 1e-14) {
                cout << func->getName() << " (Newton) converged at iteration " << (i+1) << endl;
                break;
            }
        }
    }

    cout << "Generating plot data for Secant method..." << endl;
    
    // Test Secant method for plotting
    for (const auto& func : functions_d) {
        // Get initial bracket
        auto bracket = func->getBracket();
        double x0 = bracket.first;
        double x1 = bracket.second;
        
        double fx0 = (*func)(x0);
        double fx1 = (*func)(x1);
        
        secant_plot << func->getName() << ",double," << 0 << "," << x0 << "," << abs(fx0) << endl;
        secant_plot << func->getName() << ",double," << 1 << "," << x1 << "," << abs(fx1) << endl;
        
        for (int i = 0; i < 15; i++) {
            // Check if function values are too close
            if (abs(fx1 - fx0) < 1e-12) {
                cout << "Warning: Function values too close for " << func->getName() << " at iteration " << i << endl;
                break;
            }
            
            // Secant method: x_new = x1 - f(x1) * (x1 - x0) / (f(x1) - f(x0))
            double x2 = x1 - fx1 * (x1 - x0) / (fx1 - fx0);
            
            secant_plot << func->getName() << ",double," << (i+2) << "," << x2 << "," << abs((*func)(x2)) << endl;
            
            // Check convergence
            if (abs(fx1) < 1e-13) {
                cout << func->getName() << " (Secant) converged at iteration " << (i+1) << endl;
                break;
            }
            
            if (abs(x2 - x1) < 1e-14) {
                cout << func->getName() << " (Secant) converged at iteration " << (i+1) << endl;
                break;
            }
            
            // Update for next iteration
            x0 = x1;
            x1 = x2;
            fx0 = fx1;
            fx1 = (*func)(x1);
        }
    }

    newton_plot.close();
    secant_plot.close();
    
    cout << "\nPlot data saved to:" << endl;
    cout << "- newton_plot.csv" << endl;
    cout << "- secant_plot.csv" << endl;
    cout << "\nYou can use these CSV files to create plots in Excel, Python, or other plotting tools." << endl;

    return 0;
}
