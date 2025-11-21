CXX = g++
CXXFLAGS = -std=c++17 -Wall -Wextra -O2 -Iinclude

SRC = src/logger.cpp src/computation.cpp src/experiment_runner.cpp src/matrix.cpp
OBJ = $(SRC:.cpp=.o)

STUDENT_MAIN = main.cpp

STUDENT_EXE = student_run

all: $(STUDENT_EXE)

$(STUDENT_EXE): $(OBJ) $(STUDENT_MAIN)
	$(CXX) $(CXXFLAGS) -o $@ $^

clean:
	rm -f $(OBJ) $(STUDENT_EXE) experiment_log.csv tmp_log.csv

.PHONY: all clean
