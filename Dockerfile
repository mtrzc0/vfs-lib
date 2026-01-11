# Use the latest Ubuntu image
FROM ubuntu:latest

# Prevent interactive prompts during installation
ENV DEBIAN_FRONTEND=noninteractive

# Update and install C++ tools, Doxygen, and GTest
RUN apt-get update && apt-get install -y \
    build-essential \
    gcc \
    g++ \
    cmake \
    make \
    libgtest-dev \
    doxygen \
    graphviz \
    git \
    vim \
    nano \
    && rm -rf /var/lib/apt/lists/*

# Compile GTest library
RUN cd /usr/src/gtest && \
    cmake . && \
    make && \
    cp lib/*.a /usr/lib

# Set the working directory inside the container
WORKDIR /app

# Default command to run
CMD ["/bin/bash"]
