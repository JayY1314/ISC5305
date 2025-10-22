# Compiler settings
CXX = cl
CXXFLAGS = /EHsc /std:c++17 /W3
TARGET = main.exe
SOURCE = main.cpp

# Default target - builds the main executable
all: $(TARGET)

# Build the main executable (depends on code.cpp)
$(TARGET): $(SOURCE) code.cpp
	$(CXX) $(CXXFLAGS) $(SOURCE) /Fe:$(TARGET)

# Build public tests
public: public_tests.cpp
	$(CXX) $(CXXFLAGS) public_tests.cpp /Fe:public_test.exe

# Build detailed tests
doc-tests: "doc_tests (1).cpp"
	$(CXX) $(CXXFLAGS) "doc_tests (1).cpp" /Fe:doc_test.exe

# Build all available tests
tests: public doc-tests

# Run the main executable
run: $(TARGET)
	./$(TARGET)

# Run public tests
run-public: public
	./public_test.exe

# Run detailed tests
run-doc-tests: doc-tests
	./doc_test.exe

# Run all available tests
run-all: tests
	./public_test.exe
	./doc_test.exe

# Clean up generated files
clean:
	del /Q *.exe *.obj *.pdb *.ilk *.exp *.lib *.json 2>nul || echo "Clean completed"

# Clean only executables
clean-exe:
	del /Q *.exe 2>nul || echo "Executables cleaned"

# Clean only object files
clean-obj:
	del /Q *.obj *.pdb *.ilk *.exp *.lib 2>nul || echo "Object files cleaned"

# Clean only generated JSON files
clean-json:
	del /Q *.json 2>nul || echo "JSON files cleaned"
