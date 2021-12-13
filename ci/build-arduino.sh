#!/bin/bash

# Exit immediately if a command exits with a non-zero status.
set -e

# Enable the globstar shell option
shopt -s globstar

# Make sure we are inside the github workspace
cd $GITHUB_WORKSPACE

# Create directories
mkdir $HOME/Arduino
mkdir $HOME/Arduino/libraries

# Install Arduino IDE
export PATH=$PATH:$GITHUB_WORKSPACE/bin
curl -fsSL https://raw.githubusercontent.com/arduino/arduino-cli/master/install.sh | sh
arduino-cli config init
arduino-cli core update-index --additional-urls https://arduino.esp8266.com/stable/package_esp8266com_index.json

# Install Arduino esp8266 core
arduino-cli core install esp8266:esp8266 --additional-urls https://arduino.esp8266.com/stable/package_esp8266com_index.json

# Install libraries
arduino-cli lib install ArduinoJson@6.18.5
arduino-cli lib install FastLED@3.4.0
arduino-cli lib install WiFiManager@2.0.4-beta
arduino-cli lib install NTPClient@3.2.0

arduino-cli compile --fqbn esp8266:esp8266:d1_mini ./esp8266-fastled-webserver/esp8266-fastled-webserver.ino
