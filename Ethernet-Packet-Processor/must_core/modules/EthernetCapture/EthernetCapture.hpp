#pragma once

#include <string>
#include <vector>
#include "pcap.h"
#include "../ThreadSafeQueue/ThreadSafeQueue.hpp"
#include "../EthDevice/EthDevice.hpp"
#include "../third_party/nlohmann/json.hpp"
#include "../Logger/Logger.hpp"

struct PcapSettings {
    int packetSize;
    int promiscMode;
    int captureTimeoutMs;
    int captureLoopCount;
    std::string filter;
};

template <typename T>
class EthernetCapture {
public:
    EthernetCapture(ThreadSafeQueue<T>& receiveQueue, const EthDevice& ethDevice);
    ~EthernetCapture();

    void startCapture();

private:
    ThreadSafeQueue<T>& receiveQueue_;
    EthDevice ethDevice_;
    PcapSettings settings_;
    char errorBuffer_[PCAP_ERRBUF_SIZE];

    std::vector<T> packetData_;
    std::unique_ptr<pcap_t, decltype(&pcap_close)> pcapHandle_{nullptr, &pcap_close};

    void loadPcapConfig();
    void applyPcapSettings();
    void parse_json(const std::string& filename);
    void receivePacketHandler(uint8_t* globalData, const struct pcap_pkthdr* packetHeader);
    static void packetHandlerWrapper(uint8_t* args, const struct pcap_pkthdr* header, const uint8_t* buffer);
    void receivePacketHandler(const std::vector<uint8_t>& packetBuffer); 
};

template <typename T>
EthernetCapture<T>::EthernetCapture(ThreadSafeQueue<T>& receiveQueue, const EthDevice& ethDevice)
    : receiveQueue_(receiveQueue), ethDevice_(ethDevice) {
        parse_json("capture_settings.json");
        applyPcapSettings();
     }

template <typename T>
EthernetCapture<T>::~EthernetCapture() {
    pcapHandle_.reset();
}

template <typename T>
void EthernetCapture<T>::applyPcapSettings() {
    pcapHandle_.reset(pcap_create(ethDevice_.getName().c_str(), NULL));

    pcap_set_tstamp_precision(pcapHandle_.get(), PCAP_TSTAMP_PRECISION_NANO);

    if (pcap_set_immediate_mode(pcapHandle_.get(), true) != 0) {
        Logger::getInstance().Log(LogLevel::ERROR, "Failed to set pcap immediate mode " + std::string(pcap_geterr(pcapHandle_.get())));
        return;
    }

    if (pcap_activate(pcapHandle_.get()) != 0) {
        Logger::getInstance().Log(LogLevel::ERROR, "Failed to activate pcap handler " + std::string(pcap_geterr(pcapHandle_.get())));
    }
    
    pcap_open_live(ethDevice_.getName().c_str(), settings_.packetSize, settings_.promiscMode, settings_.captureTimeoutMs, errorBuffer_);

    if (!pcapHandle_) {
        Logger::getInstance().Log(LogLevel::ERROR, "Failed to open device: " + std::string(errorBuffer_));
        throw std::runtime_error("Failed to open live device.");
    }

    struct bpf_program bpfFilter;
    if (pcap_compile(pcapHandle_.get(), &bpfFilter, settings_.filter.c_str(), 0, PCAP_NETMASK_UNKNOWN) == -1) {
        Logger::getInstance().Log(LogLevel::ERROR, "Failed to compile filter: " + std::string(pcap_geterr(pcapHandle_.get())));
        return;
    }

    if (pcap_setfilter(pcapHandle_.get(), &bpfFilter) == -1) {
        Logger::getInstance().Log(LogLevel::ERROR, "Failed to set filter: " + std::string(pcap_geterr(pcapHandle_.get())));
        pcap_freecode(&bpfFilter);
    }

    Logger::getInstance().Log(LogLevel::INFO, "PCAP filter applied");
    pcap_freecode(&bpfFilter);
}

template <typename T>
void EthernetCapture<T>::parse_json(const std::string& filename) {
    std::ifstream inputSettings(filename);

    // Check if the file could be opened.
    if (!inputSettings.is_open()) {
        std::cerr << "Failed to open file: " << filename << '\n';
        return;
    }

    // Use Nlohmann JSON library to parse the JSON data from the stream.
    nlohmann::json jsonConfig;
    inputSettings >> jsonConfig;

    // Now you can access the JSON data as follows:
    settings_.packetSize = jsonConfig["PACKET_SIZE"];
    settings_.promiscMode = jsonConfig["PROMISC"];
    settings_.captureTimeoutMs = jsonConfig["CAPTURE_READ_TIMEOUT_MS"];
    settings_.captureLoopCount = jsonConfig["PACKET_CAPTURE_LOOP_COUNT"];
    settings_.filter = jsonConfig["ETH_FILTER"];

    // Print the parsed data for verification.
    Logger::getInstance().Log(LogLevel::INFO, "Packet Size: " + std::to_string(settings_.packetSize));
    Logger::getInstance().Log(LogLevel::INFO, "Promisc: " + std::to_string(settings_.promiscMode));
    Logger::getInstance().Log(LogLevel::INFO, "Capture Read Timeout MS: " + std::to_string(settings_.captureTimeoutMs));
    Logger::getInstance().Log(LogLevel::INFO, "Packet Capture Loop Count: " + std::to_string(settings_.captureLoopCount));
    Logger::getInstance().Log(LogLevel::INFO, "ETH Filter: " + settings_.filter);
}

template <typename T>
void EthernetCapture<T>::packetHandlerWrapper(uint8_t* args, const struct pcap_pkthdr* header, const uint8_t* buffer) {
    // Convert the buffer to a vector of uint8_t.
    std::vector<uint8_t> packetBuffer(buffer, buffer + header->len);

    EthernetCapture<T>* self = reinterpret_cast<EthernetCapture<T>*>(args);

    // Call the actual packet handler function with the converted buffer.
    self->receivePacketHandler(packetBuffer);
}

template <typename T>
void EthernetCapture<T>::receivePacketHandler(const std::vector<uint8_t>& packetBuffer) {
    // Convert the buffer to a vector of char.
    std::vector<char> data(packetBuffer.begin(), packetBuffer.end());

    receiveQueue_.enqueue(data);
}

template <typename T>
void EthernetCapture<T>::startCapture() {
    Logger::getInstance().Log(LogLevel::INFO, "Starting capture on: " + ethDevice_.getName());

    if (pcap_loop(pcapHandle_.get(), settings_.captureLoopCount, packetHandlerWrapper, reinterpret_cast<uint8_t*>(this)) < 0) {
        Logger::getInstance().Log(LogLevel::ERROR, "Error capturing packets: " + std::string(pcap_geterr(pcapHandle_.get())));
    }
}

