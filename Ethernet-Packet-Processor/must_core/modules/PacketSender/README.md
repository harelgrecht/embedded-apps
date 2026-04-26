# PacketSender Module

## General Purpose

The `PacketSender` module is designed to facilitate the creation and sending of network packets. It provides a structured approach to packet handling, allowing developers to define custom packet formats and send them over various network protocols efficiently.

## Components

### PacketSender.hpp

#### Responsibility

- Define the structure and methods required for creating and managing packets.
- Provide functions to serialize and deserialize packets.
- Offer an interface to send packets through different network interfaces or protocols (e.g., UDP, TCP).

#### Example Code

```cpp
// Include necessary headers
#include "PacketSender.hpp"

int main() {
    // Create a packet sender instance
    PacketSender sender;

    // Define packet data and settings
    std::string payload = "Hello, Network!";
    PacketSettings settings;
    settings.protocol = UDP;
    settings.destination_ip = "127.0.0.1";
    settings.port = 54321;

    // Create a packet with the defined settings and payload
    Packet packet(payload, settings);

    // Send the packet
    bool success = sender.sendPacket(packet);
    
    if (success) {
        std::cout << "Packet sent successfully!" << std::endl;
    } else {
        std::cout << "Failed to send packet." << std::endl;
    }

    return 0;
}
```

### sender_settings.json
#### Responsibility
Store configuration settings for the PacketSender module.
Define default values for protocols, IP addresses, ports, and other relevant parameters.
Allow external modification of settings without altering code.
#### Example Code
```json
{
    "default_protocol": "UDP",
    "default_ip": "127.0.0.1",
    "default_port": 54321,
    "max_payload_size": 1024,
    "retries_on_failure": 3,
    "timeout_milliseconds": 1000
}
```
## Functions and Variables
### PacketSender.hpp
#### Functions
- PacketSender(): Constructor for the packet sender.
- bool sendPacket(Packet& packet): Send a given packet over the network using specified settings.
#### Variables
- PacketSettings default_settings: Holds default settings loaded from sender_settings.json.
#### Constructors
- Packet(std::string payload, PacketSettings settings): Initialize a packet with given payload and settings.
### General Things to Know
- Ensure that the network interfaces and protocols specified in sender_settings.json are supported by your environment.
- Handle exceptions and errors appropriately when sending packets.
- Update sender_settings.json as needed to adjust network parameters without modifying the codebase.
- Verify packet integrity before sending to avoid transmission issues.
### Usage Notes
- Include PacketSender.hpp in any file where you need to send packets.
- Load settings from sender_settings.json either programmatically or through a configuration loader if available.
- Always check the return status of sendPacket() to ensure that packets are sent successfully.