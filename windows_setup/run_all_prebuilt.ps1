param (
    [int]$populationSize,
    [int]$generations,
    [string]$deleteDocker = "yes"
)

# Function to check if a given value is an integer
function is-integer {
    param($value)
    return $value -match '^\d+$'
}

# Check for the number of arguments
if ($populationSize -and $generations) {
    if ((is-integer $populationSize) -and (is-integer $generations)) {
        # Values are valid integers
    }
    else {
        Write-Host "Error: populationSize and generations must be integers."
        exit 1
    }
}
elseif ($populationSize -and $generations -and ($deleteDocker -eq "yes" -or $deleteDocker -eq "no")) {
    if ((is-integer $populationSize) -and (is-integer $generations)) {
        # Values are valid integers
    }
    else {
        Write-Host "Error: populationSize and generations must be integers."
        exit 1
    }

    if ($deleteDocker -ne "yes" -and $deleteDocker -ne "no") {
        Write-Host "Error: deleteDocker must be 'yes' or 'no'."
        exit 1
    }
}
else {
    Write-Host "Usage: ./run_all.ps1 -populationSize <int> -generations <int> [-deleteDocker <yes|no>]"
    exit 1
}


Write-Host "`nRunning the Docker container...`n"
# Run the Docker container and mount the current directory
docker run -v "$pwd:/app/" -e POPULATION_SIZE="$populationSize" -e GENERATIONS="$generations" fasteiner/ubuntu-galib-neural-network


Write-Host "`nCopying the output file from the Docker container to the host...`n"
# Get the Docker container ID
$DOCKER_ID = (docker ps -lq)
# Copy the output file from the Docker container to the host
docker cp "$DOCKER_ID:/app/neural_network_output.txt" ./neural_network_output.txt

# Check for the number of arguments and if the argument is "no"
if ($deleteDocker -eq "no") {
    Write-Host "`nNot deleting the Docker container.`n"
}
else {
    Write-Host "`nDeleting the Docker container...`n"
    # Delete the Docker container
    docker rm $DOCKER_ID
}