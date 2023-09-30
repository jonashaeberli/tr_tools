#!/bin/bash

# Check if the can0 interface already exists
if [[ -e /sys/class/net/can0 ]]; then
    echo "can0 interface already exists. Exiting."
    exit 1
fi

# Load the CAN kernel modules if not already loaded
modprobe can
modprobe can-raw
modprobe can-dev
modprobe can-usb
modprobe vcan

# Set the bitrate for can0 to 100 kbps
ip link set can0 type can bitrate 1000000

# Bring up the can0 interface
ip link set up can0

echo "can0 interface is set up at 100 kbps."
