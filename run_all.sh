#!/bin/bash

echo "Building the Docker image..."
# Build the Docker image
docker build -t neural_network .


echo "Running the Docker container..."
# Run the Docker container and mount the current directory
docker run -v "$(pwd):/app/" neural_network


echo "Copying the output file from the Docker container to the host..."
# Get the Docker container ID
DOCKER_ID=$(docker ps -lq)
# Copy the output file from the Docker container to the host
docker cp "$DOCKER_ID:/app/neural_network_output.txt" ./neural_network_output.txt


# Check for the number of arguments and if the argument is "no"
if [ "$#" -eq 1 ] && [ "$1" == "no" ]; then
    echo "Not deleting the Docker container."
else
    echo "Deleting the Docker container..."
    # Delete the Docker container
    docker rm "$DOCKER_ID"
fi