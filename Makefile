EXECS=ga_nn
MPICC?=g++

# Define the output directory
OUTDIR=out

# Include directory for header files
INCLUDEDIR=include

# Source files
SRC=$(wildcard src/*.cpp)

# Create the output directory if it doesn't exist
$(OUTDIR):
	mkdir -p $(OUTDIR)

all: ${EXECS}

ga_nn: $(OUTDIR) $(SRC)
	${MPICC} -std=c++11 -I/usr/local/include/galib -I$(INCLUDEDIR) -o $(OUTDIR)/$@ $(SRC) -lga -Ofast -fpermissive

clean:
	rm -f $(OUTDIR)/*