#!/bin/bash

IMAGE_NAME="ubuntu-cpp-env"
CONTAINER_NAME="cpp-dev-container"

# 1. Build the image if it doesn't exist or needs an update
echo "🛠 Building Docker image with C++, CMake, and GTest..."
docker build -t $IMAGE_NAME .

# 2. Check if the container already exists
if [ "$(docker ps -aq -f name=${CONTAINER_NAME})" ]; then
    echo "🔄 Container already exists. Starting and attaching..."
    docker start -ai ${CONTAINER_NAME}
else
    echo "🚀 Creating and starting a new C++ dev container..."
    # Mount the current directory (pwd) to /app inside the container
    docker run -it \
        --name ${CONTAINER_NAME} \
        -v "$(pwd)":/app \
        $IMAGE_NAME
fi
