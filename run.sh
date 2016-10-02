#!/usr/bin/env bash

# Create make files
cmake CMakeLists.txt

# Restart on disconnect
while true; do
    # Build the project
    make

    # Run the project
    sudo ./column

    # Wait two seconds before restarting
    sleep 2
done
