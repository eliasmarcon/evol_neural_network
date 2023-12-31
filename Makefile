# Compiler and flags
CXX = g++
CXXFLAGS = -Wall -Wextra -std=c++11 -Ofast -fpermissive
PYTHON_VERSION = 3.10
PYTHON_INCLUDE = /usr/include/python$(PYTHON_VERSION)
PYTHON_LIB = /usr/lib/python$(PYTHON_VERSION)

# GA library paths (modify if your paths are different)
GA_INCLUDE = /usr/local/include/galib
GA_LIB = -lga

# Include directories
INCLUDEDIR = include

# Source files and object files
SRC = $(wildcard src/*.cpp)
OBJ = $(SRC:.cpp=.o)
TARGET = neural_network

# Define the output directory
OUTDIR = out

# Create the output directory if it doesn't exist
$(OUTDIR):
	mkdir -p $(OUTDIR)

# Default target
all: $(OUTDIR) $(TARGET)

# Rule to make the target
$(TARGET): $(OBJ)
	$(CXX) $(CXXFLAGS) -I$(PYTHON_INCLUDE) -I$(GA_INCLUDE) -I$(INCLUDEDIR) -o $(OUTDIR)/$(TARGET) $(OBJ) -L$(PYTHON_LIB) -lpython$(PYTHON_VERSION) $(GA_LIB)

# Rule to make the object files
%.o: %.cpp
	$(CXX) $(CXXFLAGS) -I$(PYTHON_INCLUDE) -I$(GA_INCLUDE) -I$(INCLUDEDIR) -c $< -o $@

# Clean target
clean:
	rm -f $(OUTDIR)/* $(OBJ)

.PHONY: clean all


# EXECS=ga_nn
# MPICC?=g++

# # Define the output directory
# OUTDIR=out

# # Include directory for header files
# INCLUDEDIR=include

# # Source files
# SRC=$(wildcard src/*.cpp)

# # Create the output directory if it doesn't exist
# $(OUTDIR):
# 	mkdir -p $(OUTDIR)

# all: ${EXECS}

# ga_nn: $(OUTDIR) $(SRC)
# 	${MPICC} -std=c++11 -I/usr/local/include/galib -I$(INCLUDEDIR) -o $(OUTDIR)/$@ $(SRC) -lga -Ofast -fpermissive

# clean:
# 	rm -f $(OUTDIR)/*