# Use Ubuntu 22.04 as base image
FROM ubuntu:22.04

# Set non-interactive mode for apt
ENV DEBIAN_FRONTEND=noninteractive

# Update and install required packages
RUN apt-get update && apt-get install -y \
    g++ \
    libssl-dev \
    libopencv-dev \
    wget \
    git \
    pkg-config \
    cmake \
    make \
    && rm -rf /var/lib/apt/lists/*

# Set working directory
WORKDIR /AES_ECB_mode

# Download cxxopts.hpp
RUN mkdir -p ./cpp-libs/cxxopts/include
RUN wget https://raw.githubusercontent.com/jarro2783/cxxopts/master/include/cxxopts.hpp \
    -O ./cpp-libs/cxxopts/include/cxxopts.hpp

# Open container with bash, no automatic execution
CMD ["/bin/bash"]
