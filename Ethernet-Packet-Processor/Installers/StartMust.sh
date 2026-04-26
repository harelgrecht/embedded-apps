#!/bin/bash

sudo ip route flush all && sudo ip addr flush dev eth0 && sudo ip addr flush dev eth1 && sudo ip addr flush dev eth2 && sudo ip addr flush dev eth3

# Change to the directory containing the MustV2Software program
cd /home/must/must_core/build/bin

# Run the MustV2Software program
sudo ./MustV2Software &
