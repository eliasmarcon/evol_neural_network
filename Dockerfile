FROM ubuntu:latest

ARG USERNAME=jlangela

# Set non-interactive frontend (useful for Docker builds)
ENV DEBIAN_FRONTEND noninteractive

# # Create a new user and configure sudo
# RUN useradd -m -s /bin/bash ${USERNAME} && \
#     mkdir -p /etc/sudoers.d && \
#     echo "${USERNAME} ALL=(ALL) NOPASSWD:ALL" | tee /etc/sudoers.d/${USERNAME}

# Install necessary packages
RUN apt-get update && apt-get install -y wget gcc git make g++ software-properties-common

# Installing Python 3.11 and python3-dev
RUN apt-get install -y software-properties-common
RUN add-apt-repository ppa:deadsnakes/ppa
RUN apt-get update
RUN apt-get install -y python3.11 python3.11-dev

# Download and install GALib
WORKDIR /tmp
RUN wget http://lancet.mit.edu/ga/dist/galib247.tgz && tar xzvf galib247.tgz
WORKDIR /tmp/galib247
RUN sed -i '/^CXXFLAGS\s*=/ s/$/ -O2 -fpermissive/' makevars && make && make install

# Add deadsnakes PPA for Python 3.11
RUN add-apt-repository ppa:deadsnakes/ppa
RUN apt-get update

# Install Python 3.11 and python3-dev
RUN apt-get install -y python3.11 python3.11-dev

# Install pip for Python 3.11
RUN apt-get install -y python3-pip
RUN python3.11 -m pip install --upgrade pip

# Install Python packages: numpy, pandas, tensorflow
RUN python3.11 -m pip install numpy pandas tensorflow scikit-learn

# Clean up the temporary files to reduce image size
RUN rm -rf /tmp/galib247 /tmp/galib247.tgz

# Switch to the new user
# USER ${USERNAME}

# Copy the current directory contents into the container at /app
WORKDIR /app

# COPY . /app

COPY src/main.cpp /app/src/main.cpp
COPY src/PythonCaller.cpp /app/src/PythonCaller.cpp
COPY src/neural_network.py /app/src/neural_network.py
COPY include /app/include
COPY input /app/input
COPY ./Makefile /app/Makefile
COPY ./run_neural_network.sh /app/run_neural_network.sh
COPY ./neural_network_output.txt /app/neural_network_output.txt

CMD ["bash", "/app/run_neural_network.sh"]

# # Compile the project
# RUN make all

# # Set the default command to run the neural network
# CMD ["/app/out/neural_network"]