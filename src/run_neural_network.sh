#!/bin/bash

output_file="/app/neural_network_output.txt"
output_executable="/app/out/neural_network"

# Clear the output file
> "$output_file"

# Run the make command to make the compiled program for the neural network
make all

# Check if compilation was successful for Sudoku creator
if [ $? -eq 0 ]; then
    echo -e "\nCompilation of Neural Network was successful. Running the program...\n"
        
    # Run the compiled program for Sudoku creator and append the output to the file
    "$output_executable" >> "$output_file"

    echo -e "\nResults saved into $output_file."
else
    echo -e "\nCompilation of Sudoku creator failed. Please check for errors in your C++ code.\n"
fi