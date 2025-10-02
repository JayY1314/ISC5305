#include "includes.h"

#include <iostream>
#include <memory>
#include <string>
#include <vector>
#include <fstream>
#include <iomanip>

using namespace std;

template <typename T>
class Result {
public:
    string func_name;
    string type_name;
    string method_name;
    T func_root;
    T func_residual;
    int iterations;
};

int main() {
    // Create output file
    ofstream outfile("results.txt");
    outfile << "func_name,type,method,root_7dec,root_14dec,residual,iterations" << endl;
    
    // Different solver parameters
    struct SolverParams {
        float residual_tolerance;
        float root_tolerance;
        int maxIterations;
    };
    vector<SolverParams> params = {
        {1.e-5, 1.e-7, 5},    // floats
        {1e-13, 1.e-14, 10},  // doubles
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

    // Test double precision functions
    cout << "Testing double precision functions:" << endl;
    for (const auto& func : functions_d) {
        for (const auto& solver : solvers_double) {
            double root = solver->computeRoot(*func, 1.e-3);
            double residual = func->getResidual();
            int iterations = solver->getFinalIteration();
            double verify_error = func->verify(root);
            
            cout << func->getName() << " (" << solver->getName() << "): root = " 
                 << fixed << setprecision(14) << root 
                 << ", residual = " << scientific << residual 
                 << ", verify_error = " << scientific << verify_error
                 << ", iterations = " << iterations << endl;
            
            outfile << func->getName() << ",double," << solver->getName() << ","
                   << fixed << setprecision(7) << root << ","
                   << fixed << setprecision(14) << root << ","
                   << scientific << residual << "," << iterations << endl;
        }
    }

    // Test float precision functions
    cout << "\nTesting float precision functions:" << endl;
    for (const auto& func : functions_f) {
        for (const auto& solver : solvers_float) {
            float root = solver->computeRoot(*func, 1.e-3f);
            float residual = func->getResidual();
            int iterations = solver->getFinalIteration();
            float verify_error = func->verify(root);
            
            cout << func->getName() << " (" << solver->getName() << "): root = " 
                 << fixed << setprecision(7) << root 
                 << ", residual = " << scientific << residual 
                 << ", verify_error = " << scientific << verify_error
                 << ", iterations = " << iterations << endl;
            
            outfile << func->getName() << ",float," << solver->getName() << ","
                   << fixed << setprecision(7) << root << ","
                   << fixed << setprecision(7) << root << ","
                   << scientific << residual << "," << iterations << endl;
        }
    }

    // Create verify error table
    cout << "\n" << string(80, '=') << endl;
    cout << "VERIFY FUNCTION ERROR TABLE" << endl;
    cout << string(80, '=') << endl;
    cout << left << setw(15) << "Function" 
         << setw(8) << "Type" 
         << setw(10) << "Method" 
         << setw(20) << "Root" 
         << setw(15) << "Verify Error" 
         << setw(12) << "Iterations" << endl;
    cout << string(80, '-') << endl;

    // Double precision verify errors
    for (const auto& func : functions_d) {
        for (const auto& solver : solvers_double) {
            double root = solver->computeRoot(*func, 1.e-3);
            double verify_error = func->verify(root);
            int iterations = solver->getFinalIteration();
            
            cout << left << setw(15) << func->getName()
                 << setw(8) << "double"
                 << setw(10) << solver->getName()
                 << setw(20) << fixed << setprecision(10) << root
                 << setw(15) << scientific << setprecision(3) << verify_error
                 << setw(12) << iterations << endl;
        }
    }

    // Float precision verify errors
    for (const auto& func : functions_f) {
        for (const auto& solver : solvers_float) {
            float root = solver->computeRoot(*func, 1.e-3f);
            float verify_error = func->verify(root);
            int iterations = solver->getFinalIteration();
            
            cout << left << setw(15) << func->getName()
                 << setw(8) << "float"
                 << setw(10) << solver->getName()
                 << setw(20) << fixed << setprecision(7) << root
                 << setw(15) << scientific << setprecision(3) << verify_error
                 << setw(12) << iterations << endl;
        }
    }

    cout << string(80, '=') << endl;

    outfile.close();
    cout << "\nResults saved to results.txt" << endl;

    return 0;
}
