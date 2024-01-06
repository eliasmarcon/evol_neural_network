# Use the latest Ubuntu image as the base
FROM ubuntu:latest

# Set non-interactive frontend to avoid interactive prompts during build
ENV DEBIAN_FRONTEND=noninteractive

# Update system and install necessary packages
RUN apt-get update && apt-get install -y \
    wget \
    gcc \
    git \
    make \
    g++ \
    software-properties-common

# Add the deadsnakes PPA for Python 3.11 and install Python packages
RUN add-apt-repository ppa:deadsnakes/ppa \
    && apt-get update \
    && apt-get install -y python3.11 python3.11-dev \
    && apt-get install -y python3-pip \
    && python3.11 -m pip install --upgrade pip \
    && python3.11 -m pip install numpy pandas tensorflow scikit-learn

# Download, extract, and install GALib
WORKDIR /tmp
RUN wget http://lancet.mit.edu/ga/dist/galib247.tgz \
    && tar xzvf galib247.tgz \
    && cd galib247 \
    && sed -i '/^CXXFLAGS\s*=/ s/$/ -O2 -fpermissive/' makevars \
    && make && make install

# Clean up the temporary files to reduce image size
RUN rm -rf /tmp/galib247 /tmp/galib247.tgz

# Copy the current directory contents into the container at /app
WORKDIR /app
COPY src /app
COPY include /app
COPY input /app
COPY ./Makefile /app

# Compile the project
RUN make all

# Set the default command to run the neural network
CMD ["/app/out/neural_network"]