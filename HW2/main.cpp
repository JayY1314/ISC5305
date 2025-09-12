#include <iostream>
#include <string>
#include <vector>
#include <cmath>
#include <fstream>
#include <random>

#include "homework2_skeleton.cpp"

using namespace std;

int main(int argc, char** argv)
{
    string file2d = "traject_2d.txt";
    string file3d = "traject_3d.txt";
    string file6d = "traject_6d.txt";
    
    if (argc >= 4) {
        file2d = argv[1];
        file3d = argv[2];
        file6d = argv[3];
    }

    ofstream out2d(file2d);
    ofstream out3d(file3d);
    ofstream out6d(file6d);
    
    if (!out2d || !out3d || !out6d) {
        cerr << "Failed to open output files" << endl;
        return 1;
    }

    // Random number generator for initial positions
    random_device rd;
    mt19937 gen(rd());
    uniform_real_distribution<double> dis(-1.0, 1.0);

    double t = 0.0;
    double dt = 0.02;  
    double T = 4.0;    

    // Initialize particles with random positions and zero velocity
    // 2D particle
    Vector pos2d(dis(gen), dis(gen));
    Vector vel2d(0.0, 0.0);  
    Vector force2d(0.0, 0.0);
    Particle p2d(1.0, pos2d, vel2d, force2d);

    // 3D particle
    Vector pos3d(dis(gen), dis(gen), dis(gen));
    Vector vel3d(0.0, 0.0, 0.0);  
    Vector force3d(0.0, 0.0, 0.0);
    Particle p3d(1.0, pos3d, vel3d, force3d);

    // 6D particle
    Vector pos6d(dis(gen), dis(gen), dis(gen), dis(gen), dis(gen), dis(gen));
    Vector vel6d(0.0, 0.0, 0.0, 0.0, 0.0, 0.0); 
    Vector force6d(0.0, 0.0, 0.0, 0.0, 0.0, 0.0);
    Particle p6d(1.0, pos6d, vel6d, force6d);

    // Headers
    out2d << "time x y" << endl;
    out3d << "time x y z" << endl;
    out6d << "time x y z w u v" << endl;

    cout << "Starting simulation with PDF-compliant parameters:" << endl;
    cout << "dt = " << dt << ", T = " << T << endl;
    cout << "2D initial position: " << pos2d << endl;
    cout << "3D initial position: " << pos3d << endl;
    cout << "6D initial position: " << pos6d << endl;

    // Euler integration loop
    for (t = 0.0; t <= T + 1e-12; t += dt) {
        // Log current state
        out2d << t << " " << p2d.position_[0] << " " << p2d.position_[1] << endl;
        out3d << t << " " << p3d.position_[0] << " " << p3d.position_[1] << " " << p3d.position_[2] << endl;
        out6d << t << " " << p6d.position_[0] << " " << p6d.position_[1] << " " << p6d.position_[2] 
              << " " << p6d.position_[3] << " " << p6d.position_[4] << " " << p6d.position_[5] << endl;

        // Advance particles using Euler method
        p2d.update(t, dt);
        p3d.update(t, dt);
        p6d.update(t, dt);
    }

    out2d.close();
    out3d.close();
    out6d.close();

    cout << "Simulation completed. Data saved to:" << endl;
    cout << "- " << file2d << " (2D trajectory)" << endl;
    cout << "- " << file3d << " (3D trajectory)" << endl;
    cout << "- " << file6d << " (6D trajectory)" << endl;

    return 0;
}
