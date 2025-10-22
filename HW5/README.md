# BrainMesh 3D Mesh Analysis

A C++ project for analyzing 3D brain cortical surface mesh data from VTK files.

## Features

- Read VTK format mesh data
- Calculate total surface area
- Compute vertex-centered areas
- Generate edge length statistics
- Create histograms for data visualization
- Support for different precision types (float/double)
- Modern C++ with templates and smart pointers

## Requirements

### C++ Compiler
- g++ with C++14 support
- Or any C++14 compatible compiler

### Python (for histograms)
- Python 3.6+
- matplotlib
- numpy

## Installation

1. Install Python dependencies:
```bash
pip install -r requirements.txt
```

2. Compile the C++ code:
```bash
make
```

## Usage

### Basic Usage
```bash
# Run the main program
./brain_mesh

# Generate histograms
python plot_histograms.py
```

### Test Mode
```bash
# Enable test mode in main.cpp by setting test_code = true
# Then run:
./brain_mesh
```

## Output Files

- `vertex_areas.txt`: Vertex-centered areas
- `triangle_areas.txt`: Individual triangle areas  
- `edge_lengths.txt`: Edge length statistics
- `images/edge_lengths_histogram.png`: Edge length histogram
- `images/vertex_areas_histogram.png`: Vertex area histogram

## File Structure

```
├── main.cpp                 # Main program
├── brain_mesh.h            # Class declaration
├── brain_mesh.cpp          # Class implementation
├── brain_mesh_macros.h     # Template macros
├── Makefile                # Build configuration
├── plot_histograms.py      # Python visualization script
├── Cort_lobe_poly.vtk      # Input data file
├── images/                 # Output directory for plots
├── requirements.txt        # Python dependencies
├── README.md              # This file
└── REPORT.md              # Detailed report
```

## Compilation

### Using Makefile
```bash
make          # Build the project
make clean    # Clean build files
make run      # Build and run
make test     # Build and test
```

### Manual Compilation
```bash
g++ -std=c++14 -Wall -Wextra -O2 -o brain_mesh main.cpp brain_mesh.cpp
```

## Data Format

The program expects VTK ASCII format files with:
- POINTS section containing vertex coordinates
- POLYGONS section containing triangle indices

## Verification

The program includes built-in verification:
- Sum of vertex areas equals total surface area
- Geometric calculations use standard formulas
- Test cases with known results

## License

This project is part of an academic assignment for ISC5305-SP course.
