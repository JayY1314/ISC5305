# Compiler
CXX = g++
CXXFLAGS = -Wall -g -std=c++11

# Object files
OBJS = main.o grid3d_1d_array.o grid3d_new.o grid3d_vector.o
OBJS_test = test_comprehensive.o 

# ----------------------
# Build homework target
homework.x: $(OBJS)
	$(CXX) $(CXXFLAGS) -o homework.x $(OBJS)

# Build test target
test_comprehensive.x: $(OBJS_test)
	$(CXX) $(CXXFLAGS) -o test_comprehensive.x $(OBJS_test)
# ----------------------

# Pattern rule for compiling .cpp files to .o files
%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $<

# Dependencies for main code
main.o: grid3d_1d_array.h grid3d_new.h grid3d_vector.h
grid3d_1d_array.o: grid3d_1d_array.h
grid3d_new.o: grid3d_new.h
grid3d_vector.o: grid3d_vector.h

# Dependencies for test
test_comprehensive.o: grid3d_1d_array.h grid3d_new.h grid3d_vector.h

# Clean up
clean:
	rm -f homework.x test_comprehensive.x $(OBJS) $(OBJS_test) a.out