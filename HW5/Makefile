# Makefile for BrainMesh project
# Compiler and flags
CXX = g++
CXXFLAGS = -std=c++14 -Wall -Wextra -O2

# Target executable
TARGET = brain_mesh

# Source files
SOURCES = main.cpp brain_mesh.cpp

# Object files
OBJECTS = $(SOURCES:.cpp=.o)

# Default target
all: $(TARGET)

# Build the executable
$(TARGET): $(OBJECTS)
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(OBJECTS)

# Compile source files to object files
%.o: %.cpp brain_mesh.h brain_mesh_macros.h
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Clean target - removes executable and object files
clean:
	rm -f $(TARGET) $(OBJECTS) *.txt test_file.vtk

# Run the program
run: $(TARGET)
	./$(TARGET)

# Test the program
test: $(TARGET)
	./$(TARGET)

# Phony targets
.PHONY: all clean run test
