# TunnelReceiverProcess Module

## Overview

The `TunnelReceiverProcess` module is designed to handle receiving data through a network tunnel in a multi-threaded environment. It provides a structured approach to managing network communications, ensuring that data is received efficiently and reliably.

## General Purpose

- **Network Data Reception**: Receive data packets over a network tunnel.
- **Thread Safety**: Ensure safe concurrent access to the reception process.
- **Ease of Integration**: Simplify the integration of tunnel receiving functionality into existing applications.

## Components

### TunnelReceiverProcess.hpp

**Responsibilities:**
- Defines the structure and methods needed to receive data through a network tunnel.
- Provides utility functions for managing network connections and data processing.

**Functions and Variables:**

- `TunnelReceiverProcess(const std::string& ipAddress, int port)`: Constructor to initialize the receiver with a specified IP address and port.
- `~TunnelReceiverProcess()`: Destructor to clean up resources.
- `void start()`: Starts the reception process.
- `void stop()`: Stops the reception process.
- `bool isRunning() const`: Checks if the reception process is running. Returns `true` if running, `false` otherwise.

**Example Code:**

```cpp
#include "TunnelReceiverProcess/TunnelReceiverProcess.hpp"
#include <iostream>

class MyTunnelReceiver : public TunnelReceiverProcess {
public:
    MyTunnelReceiver(const std::string& ipAddress, int port) : TunnelReceiverProcess(ipAddress, port) {}

protected:
    void onDataReceived(const std::vector<uint8_t>& data) override {
        // Implement the logic to process received data here
        std::cout << "Data received: ";
        for (auto byte : data) {
            std::cout << static_cast<int>(byte) << " ";
        }
        std::cout << std::endl;
    }
};

int main() {
    MyTunnelReceiver receiver("192.168.1.10", 5000);
    receiver.start();

    // Let the receiver run for a while
    std::this_thread::sleep_for(std::chrono::seconds(10));

    receiver.stop();
    return 0;
}
```

## Usage Guidelines

### Setting Up TunnelReceiverProcess.hpp

1. **Include the Header File**: Add `#include "TunnelReceiverProcess/TunnelReceiverProcess.hpp"` to your source file.
2. **Create a Derived Class**: Derive a class from `TunnelReceiverProcess` and implement the `onDataReceived()` method with your data processing logic.

### Start and Stop the Receiver

- Use `start()` to begin the reception process.
- Use `stop()` to terminate the reception process.

## Example Usage

1. **Create an Instance**: Instantiate your derived class with the desired IP address and port.
2. **Run the Receiver**: Call `start()` to start receiving data.
3. **Stop the Receiver**: Call `stop()` to stop the reception process after a certain period or condition is met.

## Important Notes

- **Dependencies**: Ensure all necessary network libraries (e.g., Boost.Asio) are included and linked correctly.
- **Error Handling**: Implement error handling mechanisms to manage potential issues during network communication or data processing.
- **Security Considerations**: Be cautious when handling network configurations to prevent security vulnerabilities such as injection attacks.

This module is intended for programmers who need to integrate tunnel receiving functionality into their applications. By using `TunnelReceiverProcess.hpp`, developers can efficiently manage network communications with reliability and ease.