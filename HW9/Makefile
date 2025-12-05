CXX = g++
CXXFLAGS = -std=c++11 -O3 -Wall -pthread
MPICXX = mpicxx

# Detect if OpenMP is supported (MacOS clang often needs libomp)
# Users might need to set paths: e.g., -I/opt/homebrew/include -L/opt/homebrew/lib -lomp
OMPFLAGS = -fopenmp -DOMP

TARGETS = mv_serial.x mv_threads.x mv_openmp.x mv_mpi.x

all: $(TARGETS)

mv_serial.x: matrix_vector_multiply.cpp utils.cpp
	$(CXX) $(CXXFLAGS) -o $@ $^

mv_threads.x: matrix_vector_multiply_threads.cpp utils.cpp
	$(CXX) $(CXXFLAGS) -o $@ $^

mv_openmp.x: matrix_vector_multiply_openmp.cpp utils.cpp
	$(CXX) $(CXXFLAGS) $(OMPFLAGS) -o $@ $^

mv_mpi.x: matrix_vector_multiply_mpi.cpp utils.cpp
	$(MPICXX) $(CXXFLAGS) -o $@ $^

clean:
	rm -f $(TARGETS) *.o *.x
