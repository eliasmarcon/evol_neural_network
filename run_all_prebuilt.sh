#!/bin/bash


echo -e "\nRunning the Docker container...\n"
# Run the Docker container and mount the current directory
docker run -v "$(pwd):/app/" fasteiner/ubuntu-galib-neural-network


echo -e "\nCopying the output file from the Docker container to the host...\n"
# Get the Docker container ID
DOCKER_ID=$(docker ps -lq)
# Copy the output file from the Docker container to the host
docker cp "$DOCKER_ID:/app/neural_network_output.txt" ./neural_network_output.txt


# Check for the number of arguments and if the argument is "no"
if [ "$#" -eq 1 ] && [ "$1" == "no" ]; then
    echo -e "\nNot deleting the Docker container.\n"
else
    echo -e "\nDeleting the Docker container..."
    # Delete the Docker container
    docker rm "$DOCKER_ID"
fi