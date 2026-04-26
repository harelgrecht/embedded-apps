# ThreadSafeQueue Module

## Overview

The `ThreadSafeQueue` module provides a thread-safe queue implementation suitable for use in multi-threaded applications. It ensures that multiple threads can safely enqueue and dequeue items without causing race conditions or data corruption.

## General Purpose

- **Thread Safety**: Ensure safe concurrent access to the queue by multiple threads.
- **Efficient Data Management**: Provide efficient methods for enqueuing, dequeuing, and checking queue status.
- **Ease of Integration**: Simplify the process of adding thread-safe queue functionality into existing projects.

## Components

### ThreadSafeQueue.hpp

**Responsibilities:**
- Defines the structure and methods needed to create, manipulate, and manage a thread-safe queue.
- Provides synchronization mechanisms to handle concurrent access by multiple threads.

**Functions and Variables:**

- `ThreadSafeQueue()`: Constructor to initialize an empty thread-safe queue.
- `~ThreadSafeQueue()`: Destructor to clean up resources.
- `void enqueue(const T& item)`: Adds an item to the back of the queue.
- `bool dequeue(T& item)`: Removes an item from the front of the queue. Returns `true` if successful, `false` otherwise.
- `bool try_dequeue(T& item)`: Attempts to remove an item from the front of the queue without blocking. Returns `true` if successful, `false` otherwise.
- `bool empty() const`: Checks if the queue is empty. Returns `true` if empty, `false` otherwise.
- `size_t size() const`: Returns the number of items in the queue.

**Example Code:**

```cpp
#include "ThreadSafeQueue/ThreadSafeQueue.hpp"
#include <iostream>
#include <thread>

void producer(ThreadSafeQueue<int>& queue) {
    for (int i = 0; i < 10; ++i) {
        queue.enqueue(i);
        std::cout << "Produced: " << i << std::endl;
    }
}

void consumer(ThreadSafeQueue<int>& queue) {
    int item;
    while (queue.dequeue(item)) {
        std::cout << "Consumed: " << item << std::endl;
    }
}

int main() {
    ThreadSafeQueue<int> queue;

    std::thread producerThread(producer, std::ref(queue));
    std::thread consumerThread(consumer, std::ref(queue));

    producerThread.join();
    consumerThread.join();

    return 0;
}
```

## Usage Guidelines

### Setting Up ThreadSafeQueue.hpp

1. **Include the Header File**: Add `#include "ThreadSafeQueue/ThreadSafeQueue.hpp"` to your source file.
2. **Create an Instance**: Instantiate a `ThreadSafeQueue` object of the desired type (e.g., `ThreadSafeQueue<int>`).

### Enqueue and Dequeue Items

- Use `enqueue()` to add items to the queue.
- Use `dequeue()` to remove items from the queue, ensuring thread safety.

## Example Usage

- **Producer Function**: Add items to the queue in a producer function.
- **Consumer Function**: Remove items from the queue in a consumer function.
- **Run Threads**: Create and join threads for producers and consumers to ensure proper execution.

## Important Notes

- **Type Safety**: Ensure that the type used with `ThreadSafeQueue` is appropriate for your application's needs.
- **Error Handling**: Implement error handling mechanisms if necessary, especially when dealing with complex data types or operations.
- **Performance Considerations**: Be aware of potential performance implications when using synchronization primitives in high-performance applications.

This module is intended for programmers who need to integrate thread-safe queue functionality into their multi-threaded applications. By using `ThreadSafeQueue.hpp`, developers can manage shared resources efficiently and safely across multiple threads.