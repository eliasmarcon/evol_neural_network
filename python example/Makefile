# Makefile

# Compiler settings
CXX = g++
CXXFLAGS = -Wall -Wextra -std=c++11
PYTHON_VERSION = 3.8
PYTHON_INCLUDE = /usr/include/python$(PYTHON_VERSION)
PYTHON_LIB = /usr/lib/python$(PYTHON_VERSION)

# Source and target settings
SRC = main.cpp
TARGET = main

# Define the command for compilation
$(TARGET): $(SRC)
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(SRC) -I$(PYTHON_INCLUDE) -L$(PYTHON_LIB) -lpython$(PYTHON_VERSION)

# Clean the build
clean:
	rm -f $(TARGET)

# Run the program
run: $(TARGET)
	./$(TARGET)

.PHONY: clean run
