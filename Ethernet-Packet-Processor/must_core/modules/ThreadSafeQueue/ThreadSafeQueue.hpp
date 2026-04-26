#pragma once

#include <iostream>
#include <queue>
#include <mutex>
#include <vector>
#include <condition_variable>

template <typename T>
class ThreadSafeQueue {
    public:
        ThreadSafeQueue();
        ThreadSafeQueue(size_t queueCapacity);
        ~ThreadSafeQueue() = default;
        
        ThreadSafeQueue(const ThreadSafeQueue&) = delete;
        ThreadSafeQueue& operator=(const ThreadSafeQueue&) = delete;


        void enqueue(const std::vector<T>& value);
        std::vector<T> dequeue();
        bool isEmpty();
        bool isFull();
        size_t queueSize();

    private:
        std::queue<std::vector<T>> queue_;
        std::mutex mutex_;
        std::condition_variable conditionVariable_;
        size_t queueCapacity_;

        int constexpr static DEFAULT_CAPACITY = 5000;
};

template <typename T>
ThreadSafeQueue<T>::ThreadSafeQueue() : queueCapacity_(DEFAULT_CAPACITY) { }

template <typename T>
ThreadSafeQueue<T>::ThreadSafeQueue(size_t Capacity) : queueCapacity_(Capacity) { }

template <typename T>
bool ThreadSafeQueue<T>::isEmpty() {
    return queue_.empty();
}

template <typename T>
bool ThreadSafeQueue<T>::isFull() {
    return queue_.size() >= queueCapacity_;
}

template <typename T>
size_t ThreadSafeQueue<T>::queueSize() {
    return queue_.size();
}

template <typename T>
void ThreadSafeQueue<T>::enqueue(const std::vector<T>& value) {
    std::unique_lock<std::mutex> lock(mutex_);
    conditionVariable_.wait(lock,[this]() { return !isFull(); });
    queue_.push(std::move(value));
    conditionVariable_.notify_one();
}

template <typename T>
std::vector<T> ThreadSafeQueue<T>::dequeue() {
    std::unique_lock<std::mutex> lock(mutex_);
    conditionVariable_.wait(lock,[this]() { return !isEmpty(); });
    std::vector<T> item = std::move(queue_.front()); // get the front element and move it into item
    queue_.pop(); // remove the front element
    conditionVariable_.notify_one();
    return item;
}
