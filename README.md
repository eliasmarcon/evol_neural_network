# Genetic Neural Network

This project demonstrates the use of a Genetic Algorithm (GA) to evolve the weights and biases of a neural network. The genetic algorithm optimizes the neural network's performance with the goal of achieving high accuracy on a specific task. 

## Disclaimer

**In order to run everything properly docker desktop needs to be running before executing anything!**

The project is ment to run just with a single bash script `run_all.sh` which sets up the docker container and the executes the `run_neural_network.sh` script. After the run the output file will be copied from the docker directory into the host directory. If the argument "no" is given as input for the `run_all.sh` script, the docker container will not be deleted after the results are copied to the host directory, if the argument is not provided the docker container will be deleted.

**If on Windows please make sure that the terminal you are using is a Unix-like Terminal!**


```sh
./run_all.sh # to delete the docker container after the run

./run_all.sh no # not to delete the docker container after the run 
```

## Folder Structure

- `./include`:

  - `PythonCaller.h`: 
    - **Purpose:** The PythonCaller class serves as a utility for interfacing C++ code with Python, providing functions to call Python functions, convert C++ vectors to Python lists, and handle Python object conversions.

    - **Key Features:**

      - **Function Invocation:** The CallPythonFunction static method facilitates calling a specific Python function, providing module and function names along with input arguments. It also retrieves float values for loss and accuracy.
      - **Vector Conversion:** The class includes template functions (VectorToPyList and Vector2DToPyList) to convert 1D and 2D C++ vectors to equivalent Python lists. These functions handle various data types and ensure proper memory management.
      - **Flexibility and Extensibility:** The class includes type-specific template specializations for converting different C++ data types to Python objects, offering flexibility for handling various data structures in the Python-C++ interface.

- `./input`:

  - `heart.csv`: The dataset, collected in 1988, comprises four databases named Cleveland, Hungary, Switzerland, and Long Beach V. It includes 76 attributes, with a focus on a subset of 14 attributes in published experiments. The "target" field indicates the presence of heart disease, represented as integer values 0 (no disease) and 1 (disease) in patients.


- `./out`: includes the compiled `main.cpp` and `PythonCaller.cpp` scripts

- `./src`:

  - `main.cpp`: contains the main genetic algorithm which interacts with a python neural network in order to train and evaluate the python weights and biases.

    - **Genetic Algorithm Configuration:** Configures a genetic algorithm for evolving neural network architectures, specifying parameters like population size, maximum generations, and mutation/crossover probabilities.
    
    - **Objective Function and Evaluation:** Defines an objective function for evaluating the fitness of neural network genomes based on their weights and biases. The evaluation involves communication with a Python script for performance metrics.
    
    - **Results and Visualization:** Executes the genetic algorithm, displaying the best-evolved neural network's architecture (weights and biases), accuracy, and loss. Additionally, the program measures and prints the elapsed time for the evolutionary process.

  - `neural_network.py`: 

    - **Neural Network Initialization:** Defines functions for loading a heart disease dataset, standardizing features, and creating a neural network using TensorFlow/Keras with specified layer sizes, weights, and biases.
    
    - **Model Evaluation:** Implements a function for evaluating the neural network model on the loaded dataset, considering a set of parameters including weights and biases. Uses binary cross-entropy loss and accuracy metrics.

    - **Parameter Formatting:** Provides a utility function for formatting the neural network parameters, organizing them into a structure suitable for building the model with multiple layers.

  - `PythonCaller.cpp`: 
    - **Python Integration:** Implements a C++ class PythonCaller for interacting with Python from C++ using the Python C API. Initializes the Python interpreter in the constructor and finalizes it in the destructor.
    
    - **Function Invocation:** Defines a method CallPythonFunction for invoking a specified Python function from a given module with arguments. Extracts and returns results, including loss and accuracy values, from the called Python function.
    
    - **Error Handling:** Incorporates error handling to manage potential issues during the Python function invocation, printing errors to standard error output.

- `Dockerfile`: This Dockerfile sets up an environment for a neural network project on Ubuntu. It installs essential packages, Python 3.11, and GALib. The project's source files and dependencies are copied, and the container runs a script to execute the neural network. The abstract includes user and package configurations, Python environment setup, GALib installation, cleanup, and container execution.

- `Makefile`: This Makefile configures compilation settings for a C++ neural network project. It includes rules for creating object files, the target neural network executable, and cleaning up generated files. Additionally, it specifies dependencies on Python, GA library, and includes directory. The second part, commented out, indicates an alternative approach using MPICC for a parallelized version (ga_nn).

- `neural_network_output.txt`: the output file where the result of the neural network is stored with informations about the neural network and the weights and biases as well as the evaluation results

- `run_all.sh`: This Bash script automates the Docker image building and container execution process for a neural network project. It builds the Docker image, runs the container, copies the output file from the container to the host, and deletes the container afterward.

- `run_neural_network.sh`: This Bash script compiles a neural network program, runs it, and saves the output to a designated file. It checks for successful compilation and provides appropriate messages. If compilation is successful, it executes the program and appends the results to the specified output file.

## Requirements
- C++ compiler with support for C++11 features
- Docker Desktop
- Unix System


## Usage

The easiest way to run the Neural network is to just run the `./run_all.sh` bash script which sets up docker and runs all of the needed scripts. The results will also be saved automatically to the `neural_network_output.txt` file and the docker container will automatically remove itself. If the removal of the docker file is not wanted then just add the argument "no" to the `./run_all.sh` script.

## Authors

- Elias Marcon MSc. | [@eliasmarcon](https://github.com/eliasmarcon)
- Ing. Fabian Steiner BSc.| [@fasteiner](https://github.com/fasteiner/)
- Jan Langela Regincos BSc. | [@janpilu](https://github.com/janpilu)

