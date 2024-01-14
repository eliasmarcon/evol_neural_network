param(
    [Alias("no")]
    [switch]$doNotDeleteDockerContainer
)

write-host "`nPulling the Docker image...`n"
# Pull the Docker image
docker pull fasteiner/ubuntu-galib-neural-network


write-host "`nRunning the Docker container...`n"
# Run the Docker container and mount the current directory
docker run -v "$($pwd):/app/" fasteiner/ubuntu-galib-neural-network


write-host "`nCopying the output file from the Docker container to the host...`n"
# Get the Docker container ID
$DOCKER_ID=$(docker ps -lq)
# Copy the output file from the Docker container to the host
docker cp "$DOCKER_ID`:/app/neural_network_output.txt" ./neural_network_output.txt


# Check for the number of arguments and if the argument is "no"
if ($doNotDeleteDockerContainer -ne $false) {
    write-host "`nNot deleting the Docker container.`n"
}
else{
    write-host "`nDeleting the Docker container..."
    # Delete the Docker container
    docker rm "$DOCKER_ID"
}