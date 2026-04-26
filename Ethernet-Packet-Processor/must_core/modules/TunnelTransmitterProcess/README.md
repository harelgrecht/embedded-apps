# TunnelTransmitterProcess Module

## Overview

The `TunnelTransmitterProcess` module is designed to handle transmitting data through a network tunnel in a multi-threaded environment. It provides a structured approach to managing network communications, ensuring that data is sent efficiently and reliably.

## General Purpose

- **Network Data Transmission**: Transmit data packets over a network tunnel.
- **Thread Safety**: Ensure safe concurrent access to the transmission process.
- **Ease of Integration**: Simplify the integration of tunnel transmitting functionality into existing applications.

## Components

### TunnelTransmitterProcess.hpp

**Responsibilities:**
- Defines the structure and methods needed to transmit data through a network tunnel.
- Provides utility functions for managing network connections and data processing.

**Functions and Variables:**

- `TunnelTransmitterProcess(const std::string& ipAddress, int port)`: Constructor to initialize the transmitter with a specified IP address and port.
- `~TunnelTransmitterProcess()`: Destructor to clean up resources.
- `void start()`: Starts the transmission process.
- `void stop()`: Stops the transmission process.
- `bool isRunning() const`: Checks if the transmission process is running. Returns `true` if running, `false` otherwise.
- `void sendData(const std::vector<uint8_t>& data)`: Sends data through the network tunnel.

**Example Code:**

```cpp
#include "TunnelTransmitterProcess/TunnelTransmitterProcess.hpp"
#include <iostream>
#include <thread>

class MyTunnelTransmitter : public TunnelTransmitterProcess {
public:
    MyTunnelTransmitter(const std::string& ipAddress, int port) : TunnelTransmitterProcess(ipAddress, port) {}

protected:
    void onConnectionEstablished() override {
        // Implement the logic to handle connection established event
        std::cout << "Connection established." << std::endl;
    }

    void onConnectionLost() override {
        // Implement the logic to handle connection lost event
        std::cout << "Connection lost." << std::endl;
    }
};

int main() {
    MyTunnelTransmitter transmitter("192.168.1.10", 5000);
    transmitter.start();

    // Send some data after the connection is established
    std::this_thread::sleep_for(std::chrono::seconds(2));

    std::vector<uint8_t> data = {0x01, 0x02, 0x03, 0x04};
    transmitter.sendData(data);

    // Let the transmitter run for a while
    std::this_thread::sleep_for(std::chrono::seconds(5));

    transmitter.stop();
    return 0;
}
```

## Usage Guidelines

### Setting Up TunnelTransmitterProcess.hpp

1. **Include the Header File**: Add `#include "TunnelTransmitterProcess/TunnelTransmitterProcess.hpp"` to your source file.
2. **Create a Derived Class**: Derive a class from `TunnelTransmitterProcess` and implement the `onConnectionEstablished()` and `onConnectionLost()` methods with your connection event handling logic.

### Start and Stop the Transmitter

- Use `start()` to begin the transmission process.
- Use `stop()` to terminate the transmission process.

## Example Usage

1. **Create an Instance**: Instantiate your derived class with the desired IP address and port.
2. **Run the Transmitter**: Call `start()` to start transmitting data.
3. **Send Data**: Use `sendData()` to send data through the network tunnel.
4. **Stop the Transmitter**: Call `stop()` to stop the transmission process after a certain period or condition is met.

## Important Notes

- **Dependencies**: Ensure all necessary network libraries (e.g., Boost.Asio) are included and linked correctly.
- **Error Handling**: Implement error handling mechanisms to manage potential issues during network communication or data processing.
- **Security Considerations**: Be cautious when handling network configurations to prevent security vulnerabilities such as injection attacks.

This module is intended for programmers who need to integrate tunnel transmitting functionality into their applications. By using `TunnelTransmitterProcess.hpp`, developers can efficiently manage network communications with reliability and ease.