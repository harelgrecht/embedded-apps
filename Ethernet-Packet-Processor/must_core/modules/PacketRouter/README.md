# PacketRouter Module Readme

## General Purpose
The `PacketRouter` module is a C++ template class designed for routing packets between different network interfaces in a system. It provides functionality to parse and authenticate packets, determine their source (tunnel or not), and route them to the appropriate destination queue based on the packet's source IP address, destination IP address, and payload contents. This module serves as a vital component for managing data flow within a network application that supports multiple sources and tunneling.

## Functions and Variables

### Public Member Functions
- `PacketRouter(ThreadSafeQueue<T>& receiveQueue, ThreadSafeQueue<T>& processQueue, ThreadSafeQueue<T>& fromTunnelQueue, ThreadSafeQueue<T>& guiQueue)`: Constructor that initializes the PacketRouter object with the necessary queues for packet processing.
- `void packetRouteHandler()`: Main function that handles packet routing by dequeuing packets from the receive queue, parsing them, authenticating them, and enqueuing them to the appropriate destination queue based on their source and contents.

### Private Member Functions
- `bool isFromTunnel(const std::string& srcIP, const std::string& destIP)`: Determines if a packet originates from the tunnel device based on its source IP address and destination IP address.
- `bool isFromGui(PacketInfo& udpPkt)`: Checks if a packet is intended for the graphical user interface (GUI) based on its payload contents.
- `PacketInfo parsePacket(const std::vector<T>& packet)`: Parses a raw packet into source IP address, destination IP address, and payload data.

### Variables and Constants
- `tunnelDevice_`: A member variable that stores the tunnel device object retrieved from the NetworkManager based on its role.
- `toSendProcessQueue_`, `receiveQueue_`, `fromTunnelQueue_`, and `guiQueue_`: Reference variables for thread-safe queues used for packet processing, receiving packets from the capture module, receiving packets from the tunnel, and sending packets to the GUI, respectively.
- `IPV4_HEADER_SIZE_`, `PACKET_HEADER_SIZE_`, `MIN_PACKET_SIZE_`, `SRC_IP_OFFSET_`, `DEST_IP_OFFSET_`, `SRC_PORT_OFFSET_`, `DEST_PORT_OFFSET_`, and `PAYLOAD_OFFSET_`: Configuration variables for packet parsing, loaded from the `route_settings.json` file.
- `GUI_PAYLOAD_ICD_`: A configuration variable that defines a unique identifier (magic number) to detect packets intended for the GUI.

## Responsibility
The `PacketRouter` module is responsible for managing data flow within a network application by routing packets between different sources and destinations based on their source IP address, destination IP address, and payload contents. It ensures that only valid packets are processed and routed to the appropriate queues for further handling. This module plays a critical role in maintaining the functionality of a complex network application with multiple components.

## Example of Usage
```cpp
#include "PacketRouter.hpp"

int main() {
   // Create thread-safe queues for packet processing
   ThreadSafeQueue<uint8_t> receiveQueue;  // Packets from the capture module
   ThreadSafeQueue<uint8_t> processQueue;   // Packets to be processed and sent
   ThreadSafeQueue<uint8_t> fromTunnelQueue; // Packets received from the tunnel
   ThreadSafeQueue<uint8_t> guiQueue;        // Packets to be sent to the GUI

   // Create a PacketRouter instance with the necessary queues
   PacketRouter<uint8_t> packetRouter(receiveQueue, processQueue, fromTunnelQueue, guiQueue);

   // Start the packet routing handler in a separate thread
   std::thread routerThread(&PacketRouter<uint8_t>::packetRouteHandler, &packetRouter);

   // Continue with other tasks...

   // Don't forget to join the router thread when your application exits
   routerThread.join();

   return 0;
}
```

## General Knowledge for Programmers
- Familiarity with C++ OOP concepts and template programming is recommended.
- Understanding of network programming, packet structures, and routing mechanisms is essential.
- Experience with multithreading and thread-safe data structures may be beneficial for managing packet flow within a complex network application efficiently.
- This module assumes that the route_settings.json file contains accurate configuration values for packet parsing and authentication.
- Ensure that the PacketAuthentication::authenticatePacket() function has been implemented to provide accurate validation based on your specific requirements.
