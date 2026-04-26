# RealTimeThread Module

## Overview

The `RealTimeThread` module is designed to facilitate real-time operations in a multi-threaded environment. It provides a structured approach to creating, configuring, and managing threads that need to execute at specific intervals or with precise timing requirements.

## General Purpose

- **Real-Time Operations**: Execute tasks at precise intervals or in response to specific events.
- **Thread Management**: Simplify the creation and management of real-time threads.
- **Ease of Integration**: Integrate real-time functionality into existing applications seamlessly.

## Components

### RealTimeThread.hpp

**Responsibilities:**
- Defines the structure and methods needed to create, configure, and manage real-time threads.
- Provides utility functions for handling thread timing and synchronization.

**Functions and Variables:**

- `RealTimeThread(uint64_t periodNs)`: Constructor to initialize the real-time thread with a specified period in nanoseconds.
- `start()`: Starts the execution of the real-time thread.
- `stop()`: Stops the execution of the real-time thread.
- `run()`: The main function that contains the logic to be executed periodically. This needs to be overridden by derived classes.

**Example Code:**

```cpp
#include "RealTimeThread/RealTimeThread.hpp"
#include <iostream>

class MyRealTimeTask : public RealTimeThread {
public:
    MyRealTimeTask(uint64_t periodNs) : RealTimeThread(periodNs) {}

protected:
    void run() override {
        // Implement the real-time task here
        std::cout << "Executing real-time task" << std::endl;
    }
};

int main() {
    MyRealTimeTask task(1000000000); // 1 second period
    task.start();

    // Let the task run for a while
    sleep(5);

    task.stop();
    return 0;
}
```

### RealTimeThread.cpp

**Responsibilities:**
- Implements the methods defined in `RealTimeThread.hpp`.
- Handles thread creation, timing, and synchronization internally.

## Usage Guidelines

### Setting Up RealTimeThread.hpp

1. **Include the Header File**: Add `#include "RealTimeThread/RealTimeThread.hpp"` to your source file.
2. **Create a Derived Class**: Derive a class from `RealTimeThread` and implement the `run()` method with your real-time task logic.
3. **Start and Stop Threads**:
   - Use `start()` to begin execution of the thread.
   - Use `stop()` to terminate the thread.

### Configuring Real-Time Tasks

1. **Define Task Logic**: Implement the `run()` method in your derived class to specify what should happen during each cycle.
2. **Set Period**: Provide the desired period in nanoseconds when creating an instance of your derived class.

## Important Notes

- **Thread Safety**: Ensure that shared resources are accessed safely, using mutexes or other synchronization mechanisms if necessary.
- **Timing Accuracy**: Be aware that real-time performance can be affected by system load and scheduling policies. Consider running your application on a dedicated core for improved accuracy.
- **Error Handling**: Implement error handling to manage potential issues such as thread creation failures or synchronization errors.

This module is intended for programmers who need to integrate real-time functionality into their applications. By using `RealTimeThread.hpp` and `RealTimeThread.cpp`, developers can efficiently manage real-time tasks with precision and reliability.
