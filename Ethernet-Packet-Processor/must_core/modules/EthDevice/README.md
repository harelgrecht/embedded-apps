# EthDevice Module Readme

## General Purpose
The `EthDevice` module is a C++ class designed to represent and configure network interfaces (Ethernet devices) on a system. It provides functionality for setting up IP addresses, default gateways, virtual IPs, device flags, MTUs, and more based on user-supplied settings or self-search configuration.

## Functions and Variables

### Public Member Functions
- `EthDevice(const std::string& name)`: Constructor that initializes the `EthDevice` object with a device name.
- `~EthDevice()`: Destructor that releases resources used by the `EthDevice` object, including closing the socket.
- `applySettings()`: Applies user-supplied settings to the Ethernet device.
- `isCableConnected() const`: Checks if a cable is connected to the Ethernet device at the hardware level.
- `getRole() const`: Returns the role of the Ethernet device (Source1, Source2, Source3, or Tunnel).
- `applySelfSearchConfig(const SelfSearchConfig& config)`: Updates the device configuration from a `SelfSearchConfig` object in self-search mode.
- `from_json(const nlohmann::json& j, EthDevice& ethDevice)`: JSON deserialization for normal (user) settings from user_setting.json.
- `pingIP(const std::string& ip)`: Pings a given IP address and returns the result.
- Other getter methods to retrieve device information.

### Private Member Functions
- Helper methods for applying settings, such as `setDeviceFlags()`, `updateFlag()`, `setMTU()`, `addVirtualIpIfNeeded()`, `setSelfIP()`, and `setDefaultGateway()`.
- Helper functions for converting a string to the Role enum (`parseRole()`) and vice versa (`roleToString()`).

## Responsibility
The `EthDevice` module is responsible for:
- Representing an Ethernet device with various configuration settings.
- Setting up the device's IP address, default gateway, virtual IPs, device flags, and MTU based on user-supplied settings or self-search configuration.
- Providing methods to check cable connection status and retrieve device information.

## Example of Usage
```c
#include "EthDevice.hpp"

int main() {
    // Step 1: Create an instance of EthDevice with a device name
    std::string deviceName = "eth0";
    EthDevice ethDevice(deviceName);

    // Step 2: Apply user settings or self-search configuration (for this example, let's use user settings)
    nlohmann::json j;  // Assume you have a JSON object containing the settings
    from_json(j, ethDevice);  // Deserialize settings from JSON to EthDevice object
    ethDevice.applySettings();  // Apply the settings to the Ethernet device

    // Step 3: Use the EthDevice instance to interact with the Ethernet device
    if (ethDevice.isCableConnected()) {
        std::cout << "Cable is connected.\n";
    } else {
        std::cout << "Cable is not connected.\n";
    }

    std::string ipToPing = "8.8.8.8";  // Example IP address to ping
    if (ethDevice.pingIP(ipToPing)) {
        std::cout << "Ping successful.\n";
    } else {
        std::cout << "Ping failed.\n";
    }

    return 0;
}
```

## General Knowledge for Programmers
- Familiarity with C++ OOP concepts is recommended.
- Understanding of network programming and socket operations is essential.
- Experience with JSON data serialization and parsing using the Nlohmann JSON library may be beneficial for configuring Ethernet devices based on user settings.
