#!/bin/bash

# Function to check if a given value is an integer
is_integer() {
    [[ "$1" =~ ^[0-9]+$ ]]
}

# Check for the number of arguments
if [ "$#" -eq 2 ]; then
    if is_integer "$1" && is_integer "$2"; then
        populationSize=$1
        generations=$2
        deleteDocker="yes"
    else
        echo "Error: populationSize and generations must be integers."
        exit 1
    fi
elif [ "$#" -eq 3 ]; then
    if is_integer "$1" && is_integer "$2" && [ "$3" == "yes" -o "$3" == "no" ]; then
        populationSize=$1
        generations=$2
        deleteDocker=$3
    else
        echo "Error: populationSize and generations must be integers, and deleteDocker must be 'yes' or 'no'."
        exit 1
    fi
else
    echo "Usage: ./run_all.sh <populationSize> <generations> <deleteDocker>"
    exit 1
fi

if [ "$populationSize" -lt 10 ]
then
    echo "Error: populationSize must be greater than or equal to 10."
    exit 1
fi

if [ "$generations" -lt 10 ]
then
    echo "Error: generations must be greater than or equal to 10."
    exit 1
fi

echo -e "\nBuilding the Docker image...\n"
# Build the Docker image
docker build --build-arg POPULATION_SIZE="$populationSize" --build-arg GENERATIONS="$generations" -t neural_network .


echo -e "\nRunning the Docker container...\n"
# Run the Docker container and mount the current directory
docker run -v "$(pwd):/app/" -e POPULATION_SIZE="$populationSize" -e GENERATIONS="$generations" neural_network


echo -e "\nCopying the output file from the Docker container to the host...\n"
# Get the Docker container ID
DOCKER_ID=$(docker ps -lq)
# Copy the output file from the Docker container to the host
docker cp "$DOCKER_ID:/app/neural_network_output.txt" ./neural_network_output.txt


# Check for the number of arguments and if the argument is "no"
if [ "$deleteDocker" == "no" ]; then
    echo -e "\nNot deleting the Docker container.\n"
else
    echo -e "\nDeleting the Docker container..."
    # Delete the Docker container
    docker rm "$DOCKER_ID"
fi