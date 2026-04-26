# UARTHandler Module

## Overview

The `UARTHandler` module provides a structured approach to handling Universal Asynchronous Receiver/Transmitter (UART) communications in a multi-threaded environment. It ensures efficient and reliable data transmission over UART interfaces.

## General Purpose

- **UART Communication**: Handle data transmission and reception over UART interfaces.
- **Thread Safety**: Ensure safe concurrent access to UART operations.
- **Ease of Integration**: Simplify the integration of UART functionality into existing applications.

## Components

### UARTHandler.hpp

**Responsibilities:**
- Defines the structure and methods needed to handle UART communications.
- Provides utility functions for managing UART connections and data processing.

**Functions and Variables:**

- `UARTHandler(const std::string& device, int baudRate)`: Constructor to initialize the UART handler with a specified device and baud rate.
- `~UARTHandler()`: Destructor to clean up resources.
- `bool open()`: Opens the UART connection. Returns `true` if successful, `false` otherwise.
- `void close()`: Closes the UART connection.
- `bool isOpen() const`: Checks if the UART connection is open. Returns `true` if open, `false` otherwise.
- `bool sendData(const std::vector<uint8_t>& data)`: Sends data over the UART interface. Returns `true` if successful, `false` otherwise.
- `bool receiveData(std::vector<uint8_t>& data)`: Receives data from the UART interface. Returns `true` if successful, `false` otherwise.

**Example Code:**

```cpp
#include "UARTHandler/UARTHandler.hpp"
#include <iostream>
#include <thread>

class MyUARTHandler : public UARTHandler {
public:
    MyUARTHandler(const std::string& device, int baudRate) : UARTHandler(device, baudRate) {}

protected:
    void onDataReceived(const std::vector<uint8_t>& data) override {
        // Implement the logic to process received data here
        std::cout << "Data received: ";
        for (auto byte : data) {
            std::cout << static_cast<int>(byte) << " ";
        }
        std::cout << std::endl;
    }

    void onError(const std::string& error) override {
        // Implement the logic to handle errors here
        std::cerr << "Error: " << error << std::endl;
    }
};

int main() {
    MyUARTHandler uart("/dev/ttyS0", 9600);

    if (uart.open()) {
        std::cout << "UART opened successfully." << std::endl;

        // Send some data
        std::vector<uint8_t> data = {0x01, 0x02, 0x03, 0x04};
        uart.sendData(data);

        // Let the UART handler run for a while
        std::this_thread::sleep_for(std::chrono::seconds(5));

        uart.close();
    } else {
        std::cerr << "Failed to open UART." << std::endl;
    }

    return 0;
}
```

## UARTHandler.cpp

**Responsibilities:**
- Implements the methods defined in `UARTHandler.hpp`.
- Manages the actual UART communication operations.
- Handles data transmission and reception, as well as error management.

## Usage Guidelines

### Setting Up UARTHandler.hpp

1. **Include the Header File**: Add `#include "UARTHandler/UARTHandler.hpp"` to your source file.
2. **Create a Derived Class**: Derive a class from `UARTHandler` and implement the `onDataReceived()` and `onError()` methods with your data processing and error handling logic.
3. **Open and Close the UART Connection**:
   - Use `open()` to establish the UART connection.
   - Use `close()` to terminate the UART connection.

## Example Usage

1. **Create an Instance**: Instantiate your derived class with the desired device and baud rate.
2. **Open the UART Connection**: Call `open()` to start the UART communication process.
3. **Send Data**: Use `sendData()` to send data over the UART interface.
4. **Receive Data**: Implement `onDataReceived()` to handle incoming data.
5. **Close the UART Connection**: Call `close()` to stop the UART communication process after a certain period or condition is met.

## Important Notes

- **Dependencies**: Ensure all necessary serial communication libraries (e.g., termios) are included and linked correctly.
- **Error Handling**: Implement error handling mechanisms to manage potential issues during UART communication.
- **Security Considerations**: Be cautious when handling serial port configurations to prevent security vulnerabilities.

This module is intended for programmers who need to integrate UART functionality into their applications. By using `UARTHandler.hpp` and `UARTHandler.cpp`, developers can efficiently manage serial communications with reliability and ease.