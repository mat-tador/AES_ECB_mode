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

# Download cxxopts.hpp
RUN git clone --branch v3.3.1 --depth 1 https://github.com/jarro2783/cxxopts.git /root/cpp-libs/cxxopts

WORKDIR /AES_ECB_mode
RUN git clone https://github.com/mat-tador/AES_ECB_mode.git

# Open container with bash, no automatic execution
CMD ["/bin/bash"]
