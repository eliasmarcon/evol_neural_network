# Compiler and flags
CXX = g++
CXXFLAGS = -Wall -Wextra -std=c++11
PYTHON_VERSION = 3.8
PYTHON_INCLUDE = /usr/include/python$(PYTHON_VERSION)
PYTHON_LIB = /usr/lib/python$(PYTHON_VERSION)

# Source files and object files
SRC = main.cpp PythonCaller.cpp
OBJ = $(SRC:.cpp=.o)
TARGET = neural_network

# Default target
all: $(TARGET)

# Rule to make the target
$(TARGET): $(OBJ)
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(OBJ) -I$(PYTHON_INCLUDE) -L$(PYTHON_LIB) -lpython$(PYTHON_VERSION)

# Rule to make the object files
%.o: %.cpp
	$(CXX) $(CXXFLAGS) -I$(PYTHON_INCLUDE) -c $< -o $@

# Clean target
clean:
	rm -f $(TARGET) $(OBJ)

.PHONY: clean all
