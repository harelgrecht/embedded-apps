# Must Core Project

## Overview

The **Must Core Project** is a modular system designed to handle various network operations including data transmission, reception, routing, and real-time processing. It provides a structured approach to managing these tasks efficiently and reliably in a multi-threaded environment.

## General Purpose

- **Network Communication**: Manage sending, receiving, and routing data packets over network tunnels.
- **Thread Safety**: Ensure safe concurrent access to resources.
- **Ease of Integration**: Simplify the integration of network functionalities into existing applications.

## Components

The project is organized into several modules, each responsible for a specific aspect of network operations:

1. [GPIOHandler](modules/GPIOHandler/README.md): Handles General Purpose Input Output (GPIO) operations.
2. [Logger](modules/Logger/README.md): Provides logging capabilities with different log levels.
3. [NetworkManager](modules/NetworkManager/README.md): Manages network settings and configurations.
4. [PacketSender](modules/PacketSender/README.md): Handles the transmission of packets over a network.
5. [PacketAuthentication](modules/PacketAuthentication/README.md): Ensures the integrity and authenticity of packets.
6. [PacketRouter](modules/PacketRouter/README.md): Manages data flow by routing packets to appropriate destinations.
7. [RealTimeThread](modules/RealTimeThread/README.md): Provides real-time task execution capabilities.
8. [TunnelReceiverProcess](modules/TunnelReceiverProcess/README.md): Handles receiving data through a network tunnel.
9. [TunnelTransmitterProcess](modules/TunnelTransmitterProcess/README.md): Handles transmitting data through a network tunnel.
10. [EthernetCapture](modules/EthernetCapture/README.md): Captures packets from Ethernet interfaces.
11. [InterfaceStatus](modules/InterfaceStatus/README.md): Monitors and manages the status of network interfaces.

## Example of Usage

Below is an example of how to use the main components in a `main.cpp` file:

```cpp
#include "modules/GPIOHandler/GPIOHandler.hpp"
#include "modules/Logger/Logger.hpp"
#include "modules/NetworkManager/NetworkManager.hpp"
#include "modules/PacketSender/PacketSender.hpp"
#include "modules/PacketAuthentication/PacketAuthentication.hpp"
#include "modules/PacketRouter/PacketRouter.hpp"
#include "modules/RealTimeThread/RealTimeThread.hpp"
#include "modules/third_party/nlohmann/json.hpp"
#include "modules/ThreadSafeQueue/ThreadSafeQueue.hpp"
#include "modules/TunnelReceiverProcess/TunnelReceiverProcess.hpp"
#include "modules/TunnelTransmitterProcess/TunnelTransmitterProcess.hpp"
#include "modules/EthernetCapture/EthernetCapture.hpp"
#include "modules/InterfaceStatus/InterfaceStatus.hpp"

int main() {
    Logger::getInstance("MustLog.txt").Log(LogLevel::INFO, "Start Must...");
    
    GPIOHandler::loadConfig();
    
    ThreadSafeQueue<char> recvQueue;
    ThreadSafeQueue<char> fromTunnelQueue;
    ThreadSafeQueue<char> sendQueue;

    InterfaceStatus interfaceStatus;
    interfaceStatus.StartUpLeds();

    NetworkManager networkManager;
    networkManager.parseSettings("user_setting.json", "self_search_ips.json");
    networkManager.applySettings();

    EthernetCapture<char> capture(recvQueue, networkManager.getDeviceByRole(EthDevice::Role::TUNNEL));
    PacketRouter<char> router(recvQueue, fromTunnelQueue);
    TunnelReceiverProcess<char> receiverProcess(sendQueue, fromTunnelQueue);
    PacketSender<char> sender(sendQueue);

    RealTimeThread capture_thread([&](void*) {
        capture.startCapture();
    }, nullptr);

    RealTimeThread router_thread([&](void*) {
        router.packetRouteHandler();
    }, nullptr);

    RealTimeThread recvProcess_thread([&](void*) {
        receiverProcess.processFromTunnelPackets();
    }, nullptr);

    RealTimeThread sender_thread([&](void*) {
        sender.runSender();
    }, nullptr);

    interfaceStatus.StartEthStatusMonitor();

    return 0;
}
```

## Important Notes

- **Dependencies**: Ensure all necessary libraries (e.g., Boost.Asio, nlohmann/json) are included and linked correctly.
- **Error Handling**: Implement error handling mechanisms to manage potential issues during network communication or data processing.
- **Security Considerations**: Be cautious when handling network configurations to prevent security vulnerabilities such as injection attacks.

## Updating the Code

### Fixes or Hotfixes
- Changes should be made in the `develop` branch.

### Other Changes
- Create a new branch from the `develop` branch.
  - First, do `git pull` and then `git rebase develop`.
  - Start working on your changes.
  - After completion, checkout to `develop` and merge from the last branch you worked on.

### Finalizing Changes
- Once everything works and is checked, merge the new code from `develop` into `main`.
- Add a tag to the commit in GitLab for version control.

## Starting Work on a New Project

### Create a New Branch
- Create a new branch that comes out from the `main` branch.
  - From there, the programmer can start programming and adapt the code for the specific purpose of the current project.

## Links to Module Documentation

For detailed information about each module, refer to their respective README files:

- [GPIOHandler](modules/GPIOHandler/README.md)
- [Logger](modules/Logger/README.md)
- [NetworkManager](modules/NetworkManager/README.md)
- [PacketSender](modules/PacketSender/README.md)
- [PacketAuthentication](modules/PacketAuthentication/README.md)
- [PacketRouter](modules/PacketRouter/README.md)
- [RealTimeThread](modules/RealTimeThread/README.md)
- [TunnelReceiverProcess](modules/TunnelReceiverProcess/README.md)
- [TunnelTransmitterProcess](modules/TunnelTransmitterProcess/README.md)
- [EthernetCapture](modules/EthernetCapture/README.md)
- [InterfaceStatus](modules/InterfaceStatus/README.md)

This project structure ensures modularity, maintainability, and scalability. By following the guidelines provided, developers can efficiently integrate and extend the functionality of this core system.
