#!/bin/bash

# Exit immediately if a command exits with a non-zero status.
set -e

# Make sure we are inside the github workspace
cd $GITHUB_WORKSPACE

# Install PlatformIO CLI
export PATH=$PATH:~/.platformio/penv/bin
curl -fsSL https://raw.githubusercontent.com/platformio/platformio-core-installer/master/get-platformio.py -o get-platformio.py
python3 get-platformio.py

# Install esp8266 platform
pio platform install "espressif8266"

# Compile project
if [[ ! -v FIB_PRODUCT ]]; then
    pio run
    pio run --target buildfs
else
    pio run --environment ${FIB_PRODUCT}
    pio run --target buildfs --environment ${FIB_PRODUCT}
fi
