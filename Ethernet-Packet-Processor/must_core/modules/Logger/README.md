# Logger Module Readme

## General Purpose
The `Logger` module is a C++ class designed for logging messages with different severity levels (INFO and ERROR) to both a log file and the console. It provides a thread-safe, singleton-based logger that can be easily integrated into various parts of your application.

## Functions and Variables

### Public Member Functions
- `static Logger& getInstance(const std::string& logPath)`: Returns the single instance of the Logger class and sets the log file path if not already set.
- `static Logger& getInstance()`: Returns the single instance of the Logger class (requires setting the log file path using `getInstance(logPath)`).
- `void Log(LogLevel level, const std::string& message)`: Logs a message with a specified severity level.

### Private Member Functions
- `Logger(const std::string& logPath)`: Constructor that initializes the logger with a log file path and opens the log file for writing.
- `~Logger()`: Destructor that closes the log file if it's open.
- `std::string getSystemUptime() const`: Returns the system uptime as a formatted string (HH:MM:SS).

### Variables and Constants
- `LogLevel levelStr[]`: An array of strings representing severity levels (INFO, ERROR).
- `SECONDS_PER_HOUR`, `SECOND_PER_MINUTE`, and `DIGITS_FOR_FORMAT`: Constant expressions for time calculations.
- `logFile_`, `logPath_`, `logMutex_`: Member variables for the log file stream, log file path, and mutex for thread safety.
- `instance_` and `onceFlag_`: Static member variables for the singleton instance and synchronization flag.

## Responsibility
The `Logger` module is responsible for logging messages with different severity levels to a log file and optionally the console. It ensures thread-safe operations, provides a simple API for logging messages, and supports formatting messages with system uptime.

## Example of Usage
```cpp
#include "Logger.hpp"

int main() {
   // Get or create the logger instance and set the log file path
   Logger& logger = Logger::getInstance("/path/to/logfile.txt");

   // Log messages with different severity levels
   logger.Log(LogLevel::INFO, "This is an informational message.");
   logger.Log(LogLevel::ERROR, "An error occurred.");

   return 0;
}
```

## General Knowledge for Programmers
- Familiarity with C++ OOP concepts and singleton design pattern is recommended.
- Understanding of thread safety and file I/O operations is essential.
- This module assumes that you have the necessary permissions to write log files to the specified path.