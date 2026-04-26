#pragma once

#include <iostream>
#include <vector>
#include "../ThreadSafeQueue/ThreadSafeQueue.hpp"

constexpr size_t HEADER_SIZE = 8;

template <typename T>
class TunnelTransmitterProcess {
    public:
        TunnelTransmitterProcess(ThreadSafeQueue<T>& toTunnelProcessQueue, ThreadSafeQueue<T>& sendQueue);

         void processToTunnelPackets();

    private:
        std::vector<T> rawPacketBuffer_;
        std::vector<T> payloadBuffer_;
        
        ThreadSafeQueue<T>& toTunnelProcessQueue_;
        ThreadSafeQueue<T>& sendQueue_;

        void getPayload();
};

template <typename T>
TunnelTransmitterProcess<T>::TunnelTransmitterProcess(ThreadSafeQueue<T>& toTunnelProcessQueue, ThreadSafeQueue<T>& sendQueue) 
    : toTunnelProcessQueue_(toTunnelProcessQueue), sendQueue_(sendQueue) { }

template <typename T>
void TunnelTransmitterProcess<T>::getPayload() {
    rawPacketBuffer_ = toTunnelProcessQueue_.dequeue();
    if (rawPacketBuffer_.size() > HEADER_SIZE) {
        payloadBuffer_.assign(rawPacketBuffer_.begin() + HEADER_SIZE, rawPacketBuffer_.end());
    }
}

template <typename T>
void TunnelTransmitterProcess<T>::processToTunnelPackets() {
    /*
        Perform any necessary transformations on payloadBuffer_, then pass to the next queue.
        Data transformation or manipulation code goes here...
   */
    if (payloadBuffer_.size() > 0){
        sendQueue_.enqueue(payloadBuffer_);
    }
}
