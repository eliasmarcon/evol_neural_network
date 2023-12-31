FROM ubuntu:latest

ARG USERNAME=jlangela

# Set non-interactive frontend (useful for Docker builds)
ENV DEBIAN_FRONTEND noninteractive

# Create a new user and configure sudo
RUN useradd -m -s /bin/bash ${USERNAME} && \
    mkdir -p /etc/sudoers.d && \
    echo "${USERNAME} ALL=(ALL) NOPASSWD:ALL" | tee /etc/sudoers.d/${USERNAME}

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
RUN python3.11 -m pip install numpy pandas tensorflow

# Switch to the new user
USER ${USERNAME}

# Set the default command to bash
CMD ["/bin/bash"]
