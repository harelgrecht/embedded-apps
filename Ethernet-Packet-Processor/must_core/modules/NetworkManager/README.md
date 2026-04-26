# NetworkManager Module Readme

## General Purpose
The `NetworkManager` module is a C++ class designed for managing network interfaces (Ethernet devices) on a system. It allows you to configure and manage multiple Ethernet devices based on user settings, including normal device settings and self-search configurations. This module provides functionality for applying settings, setting system routes, running self-search routines in parallel, and retrieving devices by their role.

## Functions and Variables

### Public Member Functions
- `NetworkManager()`: Constructor that initializes the NetworkManager object.
- `parseSettings(const std::string& userSettingsFile, const std::string& selfSearchFile)`: Parses settings from a user settings file (JSON format) and optionally a self-search configurations file.
- `applySettings()`: Based on the enableSelfSearch flag, either applies normal device settings or runs self-search routines.
- `getDeviceByRole(EthDevice::Role role)`: Retrieves an Ethernet device by its role (Source1, Source2, Source3, Tunnel).
- `getEthDevices()`: Returns a reference to the array of EthDevice objects managed by the NetworkManager.

### Private Member Functions
- `setSystemRoutes()`: (Placeholder) Sets system-wide routes for normal device settings.
- `runSelfSearchInParallel()`: Runs self-search routines in parallel for all Ethernet devices.
- `runSelfSearchForDevice(EthDevice& device, const std::vector<SelfSearchConfig>& configs)`: Runs the self-search routine for a specific Ethernet device using provided configurations.

### Variables and Constants
- `ethDevices_`: A static array of EthDevice objects managed by the NetworkManager.
- `enableSelfSearch`: A flag indicating whether self-search mode is enabled (default is false).
- `selfSearchConfigs_`: In self-search mode, a vector of SelfSearchConfig objects loaded from the self-search configurations file.
- `DEVICE_NAME_PREFIX`: A constant string used as the prefix for Ethernet device names ("eth").

## Responsibility
The `NetworkManager` module is responsible for managing network interfaces (Ethernet devices) on a system. It provides functionality for parsing settings, applying configurations, running self-search routines in parallel, and retrieving devices by their role.

## Example of Usage
```cpp
#include "NetworkManager.hpp"

int main() {
   NetworkManager networkManager;

   // Parse settings from user_setting.json and optionally self_search_ips.json
   std::string userSettingsFile = "/path/to/user_setting.json";
   std::string selfSearchFile = "/path/to/self_search_ips.json";
   networkManager.parseSettings(userSettingsFile, selfSearchFile);

   // Apply settings or run self-search based on the enableSelfSearch flag
   networkManager.applySettings();

   // Retrieve a device by its role
   EthDevice source1Device = NetworkManager::getDeviceByRole(EthDevice::Role::SOURCE1);

   return 0;
}
```

## General Knowledge for Programmers
- Familiarity with C++ OOP concepts is recommended.
- Understanding of network programming and JSON data serialization/deserialization is essential.
- Experience with multithreading may be beneficial for running self-search routines in parallel.