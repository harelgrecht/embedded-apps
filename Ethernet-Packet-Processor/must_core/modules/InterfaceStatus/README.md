# Interface Status Monitoring in MUST Core
This document provides an overview of the interface status monitoring functionality within the MUST core system. There are two primary methods available: using C++ code (`InterfaceStatus.cpp`) or a shell script (`InterfaceStatus.sh`). Both methods monitor and visualize the network connection status through LEDs on compatible hardware, with the green LED indicating data transmission and the yellow LED signifying an active network connection.

## Method 1: Using `InterfaceStatus.cpp` in C++
The `InterfaceStatus.cpp` file implements a class, `InterfaceStatus`, which manages the interface status monitoring. This method requires access to the system's file streams and the Nlohmann JSON library for parsing JSON configuration files. Here are the primary functions:

- `StartEthStatusMonitor`: Starts a new thread that continuously updates the Ethernet status.
- `updateEthStatus`: Periodically checks the cable connection status of each interface, updating the LED states accordingly.
- `StartUpLeds`: Executes an initial startup sequence for all interfaces' LEDs.
## Method 2: Using `InterfaceStatus.sh` in Shell Scripting
The `InterfaceStatus.sh` script provides a shell-based solution for monitoring interface status and controlling LEDs. This method does not require any external libraries or dependencies beyond basic shell utilities and access to the system's file streams. Here are the primary functions:

- `is_cable_connected`: Checks if a cable is connected to an Ethernet device.
- `update_eth_status`: Continuously monitors network traffic and updates LED states based on connection status and activity.
- `startup_leds`: Executes an initial startup sequence for all interfaces' LEDs.

---

### How to use ?
#### C++ Method (`InterfaceStatus.cpp`)
1. Include the necessary headers and create an instance of the `InterfaceStatus` class in your main application code (`main.cpp`).
2. Call the `StartEthStatusMonitor` function to begin monitoring interface status and controlling LEDs or uncomment the comment parts in the `main.cpp`.


#### Shell Script Method (`InterfaceStatus.sh`)
```bash

#create a InterfaceStatus.service under /etc/systemd/system/InterfaceStatus.service and implement the next service.
[Unit]
Description=Interface Status monitoring service
After=network.target

[Service]
ExecStart=/home/must/InterfaceStatus.sh
Restart=on-failure
User=root

[Install]
WantedBy=multi-user.target
```

Enable the service in `/etc/systemd/system/` using to start the script on boot.
```bash
sudo chmod 777 InterfaceStatus.sh
sudo systemctl daemon-reload
sudo systemctl enable interfacestatus.service # to enable the script on boot
sudo systemctl start interfacestatus.service # to start the script from the current point

sudo systemctl disable interfacestatus.service # to disable the script on boot
sudo systemctl stop interfacestatus.service # to stop the script from the current point

```
