# GPIOHandler Module Readme

## General Purpose
The `GPIOHandler` module is a C++ class designed for handling General-Purpose Input/Output (GPIO) pins on an embedded system. It allows you to configure, control, and interact with GPIO pins using simple methods such as setting them high or low, blinking LEDs, and more.

## Functions and Variables

### `GPIOSettings` Struct
- **Purpose**: Stores configuration settings for the GPIO handler.
- **Variables**:
 - `GPIO_ON`: Value to set a GPIO pin high (usually 1).
 - `GPIO_OFF`: Value to set a GPIO pin low (usually 0).
 - `GPIO_EXPORT_PATH`: File path for exporting a GPIO pin.
 - `GPIO_UNEXPORT_PATH`: File path for unexporting a GPIO pin.
 - `GPIO_DIRECTION_PATH`: File path template for setting the direction of a GPIO pin (out, in).
 - `GPIO_VALUE_PATH`: File path template for setting the value of a GPIO pin (high or low).

### Public Member Functions
- `GPIOHandler(int ledPin, const std::string& direction)`: Constructor that initializes the GPIO handler with a LED pin number and direction.
- `~GPIOHandler()`: Destructor that unexports the GPIO pin.
- `setHigh()`: Sets the GPIO pin high.
- `setLow()`: Sets the GPIO pin low.
- `blinkLed()`: Blinks the LED connected to the GPIO pin.
- `getPinNumber() const noexcept`: Returns the GPIO pin number.
- `static void setConfigPath(const std::string& configPath)`: Sets the configuration file path (default is "GPIO_settings.json").
- `static void loadConfig()`: Loads the configuration settings from the JSON file.

### Private Member Functions
- `writeToFile(const std::string& filePath, const std::string& data)`: Writes data to a specified file path.
- `formatPath(const std::string pathTemplate) const`: Formats a file path template with the GPIO pin number.
- `static void parseJson()`: Parses JSON data from the configuration file and populates the `GPIOSettings` struct.

## Responsibility
The `GPIOHandler` module is responsible for:
- Exporting, configuring, and unexporting GPIO pins.
- Setting GPIO pin directions (input or output).
- Controlling GPIO pin values (high or low) and blinking LEDs connected to GPIO pins.
- Loading configuration settings from a JSON file.

## Example of Usage
```cpp
#include "GPIOHandler.hpp"

int main() {
   // Set the configuration file path (optional, default is "GPIO_settings.json")
   GPIOHandler::setConfigPath("GPIO_settings.json");

   // Create a GPIO handler instance for LED pin 17 in output direction
   int ledPinNumber = 17;
   std::string direction = "out";
   GPIOHandler led(ledPinNumber, direction);

   // Blink the LED three times
   for (int i = 0; i < 3; ++i) {
       led.blinkLed();
   }

   return 0;
}
```

## General Knowledge for Programmers
- Familiarity with C++ OOP concepts is recommended.
- Understanding of GPIO pins and their usage is essential.
- Experience with JSON data serialization and parsing using the Nlohmann JSON library may be beneficial for configuring GPIO settings.
- This module assumes that you have the necessary permissions to access and modify GPIO pins on your system.