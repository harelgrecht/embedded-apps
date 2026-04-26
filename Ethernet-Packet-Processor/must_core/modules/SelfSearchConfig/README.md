# SelfSearchConfig Module

## Overview

The `SelfSearchConfig` module is designed to manage configuration settings related to self-search operations in a networked application. It provides a structured approach to loading, storing, and accessing configuration data from JSON files, ensuring that the application can dynamically adjust its behavior based on these settings.

## General Purpose

- **Configuration Management**: Load and manage configuration settings for self-search operations.
- **JSON Integration**: Use `self_search_ips.json` for configuring IP addresses and other related parameters.
- **Ease of Integration**: Simplify the process of integrating configuration management into existing projects.

## Components

### SelfSearchConfig.hpp

**Responsibilities:**
- Defines the structure and methods needed to load, store, and access self-search configuration settings.
- Provides utility functions for handling JSON data and network operations.

**Functions and Variables:**

- `SelfSearchConfig(const std::string& configFilePath)`: Constructor to initialize the configuration with a specified JSON file path.
- `loadConfig()`: Loads the configuration settings from the JSON file.
- `getIPAddresses()`: Returns a list of IP addresses configured for self-search operations.
- `getPort()`: Returns the port number configured for self-search operations.

**Example Code:**

```cpp
#include "SelfSearchConfig/SelfSearchConfig.hpp"
#include <iostream>

int main() {
    SelfSearchConfig config("modules/self_search_ips.json");
    if (config.loadConfig()) {
        auto ipAddresses = config.getIPAddresses();
        int port = config.getPort();

        std::cout << "Port: " << port << std::endl;
        for (const auto& ip : ipAddresses) {
            std::cout << "IP Address: " << ip << std::endl;
        }
    } else {
        std::cerr << "Failed to load configuration." << std::endl;
    }
    return 0;
}
```

### SelfSearchConfig.cpp

**Responsibilities:**
- Implements the methods defined in `SelfSearchConfig.hpp`.
- Handles JSON parsing and data storage internally.

### self_search_ips.json

**Responsibilities:**
- Contains configuration settings for IP addresses and other parameters related to self-search operations.
- Allows dynamic adjustment of network settings without modifying the source code.

**Struct:**
```json
[
    {
        "selfIp": "192.168.1.20", // Self ip when searching for this configuration 
        "ipToPing": "192.168.1.10", // IP to ping to validate if the configuration are correct 
        // The rest are the configurations
        "destIpAddresses": ["192.168.1.50"],
        "srcPorts": [82523],
        "destPorts": [63535],
        "defaultGateway": "192.168.1.254",
        "subnetMask": "255.255.255.0",
        "remoteIp": "0.0.0.0",
        "remoteIpDestination": "0.0.0.0",
        "role": "source1"
    }
]
```
## Usage Guidelines

### Setting Up SelfSearchConfig.hpp

1. **Include the Header File**: Add `#include "SelfSearchConfig/SelfSearchConfig.hpp"` to your source file.
2. **Initialize the Configurator**: Create an instance of `SelfSearchConfig` with the path to your JSON configuration file.
3. **Load and Access Configuration**:
   - Use `loadConfig()` to load settings from the JSON file.
   - Use `getIPAddresses()`, `getPort()`, and other relevant methods to access specific configuration data.

### Configuring self_search_ips.json

1. **Create/Edit JSON File**: Open or create `self_search_ips.json` and define your desired IP addresses and port using the following structure:

    ```json
    [
        {
            "selfIp": "192.168.1.20",
            "ipToPing": "192.168.1.10",
            "destIpAddresses": ["192.168.1.50"],
            "srcPorts": [82523],
            "destPorts": [63535],
            "defaultGateway": "192.168.1.254",
            "subnetMask": "255.255.255.0",
            "remoteIp": "0.0.0.0",
            "remoteIpDestination": "0.0.0.0",
            "role": "source1"
        },
        {
            "selfIp": "192.168.2.20",
            "ipToPing": "192.168.2.10",
            "destIpAddresses": ["192.168.2.50"],
            "srcPorts": [82523],
            "destPorts": [63535],
            "defaultGateway": "192.168.2.254",
            "subnetMask": "255.255.255.0",
            "remoteIp": "0.0.0.0",
            "remoteIpDestination": "0.0.0.0",
            "role": "tunnel"
        }
    ]
    ```

2. **Use in Code**: Ensure the path provided when creating `SelfSearchConfig` points to the correct JSON file location.

## Important Notes

- **Dependencies**: Ensure all necessary libraries for JSON parsing (e.g., nlohmann/json) are included and linked correctly.
- **Error Handling**: Implement error handling mechanisms to manage potential issues during configuration loading or data access.
- **Security Considerations**: Be cautious when handling network configurations to prevent security vulnerabilities such as injection attacks.

This module is intended for programmers who need to integrate self-search functionality into their applications. By using `SelfSearchConfig.hpp`, `SelfSearchConfig.cpp`, and `self_search_ips.json`, developers can efficiently manage configuration settings with flexibility and ease.
///
