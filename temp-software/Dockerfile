FROM ubuntu:latest

# Prevent interactive prompts during package installation
ENV DEBIAN_FRONTEND=noninteractive

# Install core build tools, the AVR toolchain, and avrdude
RUN apt-get update && apt-get install -y \
    build-essential \
    gcc-avr \
    binutils-avr \
    avr-libc \
    avrdude \
    && rm -rf /var/lib/apt/lists/*

WORKDIR /app