# EthernetCapture Module Readme

## General Purpose
The EthernetCapture module is a C++ template class designed for capturing ethernet packets from a specified network interface. It utilizes the libpcap library to perform low-level packet capture and provides functionality for applying filters and processing captured packets.

## Functions and Variables

### `PcapSettings` Struct
- **Purpose**: Stores configuration settings for the pcap handle.
- **Variables**:
 - `packetSize`: Maximum size of packets to capture.
 - `promiscMode`: Promiscuous mode flag (1 for enabled, 0 for disabled).
 - `captureTimeoutMs`: Timeout value in milliseconds for packet capture.
 - `captureLoopCount`: Number of packets to capture before stopping (-1 for infinite).
 - `filter`: Filter expression for capturing specific packets.

### Constructor (`EthernetCapture<T>::EthernetCapture`)
- **Purpose**: Initializes the EthernetCapture object with a receive queue and an ethernet device.
- **Parameters**:
 - `receiveQueue`: Reference to a thread-safe queue for storing captured packets.
 - `ethDevice`: Instance of EthDevice representing the network interface.

### Destructor (`EthernetCapture<T>::~EthernetCapture`)
- **Purpose**: Releases resources used by the EthernetCapture object.

### Private Member Functions
- `loadPcapConfig()`: Loads pcap configuration settings from a JSON file.
- `applyPcapSettings()`: Applies the loaded pcap configuration settings to the pcap handle.
- `parse_json(const std::string& filename)`: Parses JSON data from a file and populates the PcapSettings struct.
- `receivePacketHandler(uint8_t* globalData, const struct pcap_pkthdr* packetHeader)`: Deprecated function for handling captured packets.
- `packetHandlerWrapper(uint8_t* args, const struct pcap_pkthdr* header, const uint8_t* buffer)`: Static wrapper function for the packet handler callback.
- `receivePacketHandler(const std::vector<uint8_t>& packetBuffer)`: Handles captured packets and enqueues them to a receive queue.
- `startCapture()`: Starts capturing packets on the specified ethernet device.

## Responsibility
The EthernetCapture module is responsible for:
- Initializing a pcap handle with the specified network interface and configuration settings.
- Applying filters to capture specific packets.
- Processing captured packets and enqueuing them to a thread-safe queue for further processing.

## Example of Usage
```c
#include "EthDevice.hpp"
#include "ThreadSafeQueue.hpp"
#include "EthernetCapture.hpp"

int main() {
    // Step 1: Create an instance of EthDevice representing the network interface
    std::string deviceName = "eth0";  // Replace with your actual network interface name
    EthDevice ethDevice(deviceName);

    // Step 2: Create a thread-safe queue to store captured packets
    ThreadSafeQueue<std::vector<char>> receiveQueue;

    // Step 3: Create an instance of EthernetCapture, passing the receive queue and ethdevice as parameters
    EthernetCapture<std::vector<char>> ethernetCapture(receiveQueue, ethDevice);

    // Step 4: Call the startCapture() function to begin capturing packets
    ethernetCapture.startCapture();

    // Now you can process the captured packets from the receive queue as needed.

    return 0;
}
```

## General Knowledge for Programmers
- Familiarity with C++ templates and OOP concepts is recommended.
- Understanding of libpcap library usage and packet filtering expressions is essential.
- Experience with thread-safe programming techniques may be beneficial for handling captured packets.
- Knowledge of JSON data serialization and parsing using the Nlohmann JSON library is useful for configuring pcap settings.
