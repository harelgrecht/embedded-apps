#pragma once

#include <string>
#include <cstdint>
#include <cstring>
#include <stdexcept>
#include <arpa/inet.h>
#include <netinet/ip.h>
#include <netinet/udp.h>
#include <netinet/ether.h>
#include <sys/socket.h>
#include <unistd.h>
#include <vector>
#include <iostream>
#include "../ThreadSafeQueue/ThreadSafeQueue.hpp"
#include "../NetworkManager/NetworkManager.hpp"
#include "../Logger/Logger.hpp"

// Define a struct to encapsulate these variables
typedef struct {
    size_t PACKET_HEADER_SIZE;
    size_t UDP_HEADER_SIZE;
    size_t IPV4_HEADER_SIZE;
    uint8_t IP_VERSION;
    uint8_t IHL; // Internet Header Length
    uint8_t DEFAULT_TTL; // Time To Live
    uint8_t PROTOCOL_TYPE_INDEX;
    uint16_t FLAGS_FRAGMENT;
    uint8_t DEFAULT_TOS; // Type of Service
    uint16_t PACKET_CHECKSUM; // UDP checksum is optional for IPv4
    int INVALID_SOCKET;
    uint16_t INITIAL_IP_CHECKSUM;
} SendConfig;

    
constexpr int ICD_INDEX = 4;
constexpr int CONVENTION_MASK = 64;
constexpr int CONVENTION_OFFSET = 6;

template <typename T>
class PacketSender {
public:
    PacketSender(ThreadSafeQueue<T>& queue);
    ~PacketSender();

    void runSender();

private:
    int packetID_ = 0;
    std::vector<T> payloadBuffer_;
    std::vector<T> packetBuffer_;
    SendConfig senderConfig_;
    ThreadSafeQueue<T>& toTunnelQueue_;
    EthDevice ethDevice_;

    std::string currentDestIp;
    int currentDestPort;
    int currentSrcPort;
    int totalPacketSize_;

    void sendPacket();
    void assemblePacket();
    void createProtocolHeader(struct udphdr* udpHeader);
    void createIpHeader(struct iphdr* ipHeader);
    void createEthHeader(struct ethhdr* ethHeader);
    uint16_t computeChecksum(uint8_t* data, int len);
    EthDevice extract_device(std::vector<T> buffer);
    void loadConfig();
};

template <typename T>
PacketSender<T>::PacketSender(ThreadSafeQueue<T>& queue) : toTunnelQueue_(queue) {
    loadConfig();
}

template <typename T>
PacketSender<T>::~PacketSender() {
    Logger::getInstance().Log(LogLevel::INFO, "Packet sender stop and finish to send");
}

template <typename T>
void PacketSender<T>::runSender() {
    while(true) {
        payloadBuffer_ = toTunnelQueue_.dequeue();
        ethDevice_ = extract_device(payloadBuffer_);

        for(int index = 0; index < ethDevice_.getDestIpList().size(); index++){
            currentDestIp = ethDevice_.getDestIpList().at(index);
            currentDestPort = ethDevice_.getDestPortList().at(index);
            currentSrcPort = ethDevice_.getSrcPortList().at(index);

            assemblePacket();
            sendPacket();
        }
    }
}

template <typename T>
void PacketSender<T>::loadConfig() {
    std::ifstream configFile("sender_settings.json");
    if (!configFile.is_open()) {
        throw std::runtime_error("Failed to open configuration file: sender_setting.json");
    }
    nlohmann::json jsonConfig;
    configFile >> jsonConfig;

    // Extract values from the JSON object
    senderConfig_.PACKET_HEADER_SIZE = jsonConfig.at("HEADER_SIZE").get<size_t>();
    senderConfig_.UDP_HEADER_SIZE = jsonConfig.at("UDP_HEADER_SIZE").get<size_t>();
    senderConfig_.IPV4_HEADER_SIZE = jsonConfig.at("IPV4_HEADER_SIZE").get<size_t>();
    senderConfig_.IP_VERSION = jsonConfig.at("IP_VERSION").get<uint8_t>();
    senderConfig_.IHL = jsonConfig.at("IHL").get<uint8_t>();
    senderConfig_.DEFAULT_TTL = jsonConfig.at("DEFAULT_TTL").get<uint8_t>();
    senderConfig_.PROTOCOL_TYPE_INDEX = jsonConfig.at("PROTOCOL_TYPE_INDEX").get<uint8_t>();
    senderConfig_.FLAGS_FRAGMENT = jsonConfig.at("FLAGS_FRAGMENT").get<uint16_t>();
    senderConfig_.DEFAULT_TOS = jsonConfig.at("DEFAULT_TOS").get<uint8_t>();
    senderConfig_.PACKET_CHECKSUM = jsonConfig.at("CHECKSUM").get<uint16_t>();
    senderConfig_.INVALID_SOCKET = jsonConfig.at("INVALID_SOCKET").get<int>();
    senderConfig_.INITIAL_IP_CHECKSUM = jsonConfig.at("INITIAL_IP_CHECKSUM").get<uint16_t>();
}

template <typename T>
void PacketSender<T>::createProtocolHeader(struct udphdr* udpHeader) {
    udpHeader->source = htons(currentSrcPort);
    udpHeader->dest = htons(currentDestPort);
    udpHeader->len = htons(senderConfig_.UDP_HEADER_SIZE + payloadBuffer_.size());
    udpHeader->check = computeChecksum(reinterpret_cast<uint8_t*>(udpHeader), senderConfig_.UDP_HEADER_SIZE);
}

template <typename T>
void PacketSender<T>::createEthHeader(struct ethhdr* ethHeader) {
    ethHeader->h_source = inet_addr(ethDevice_.getSelfIp().c_str());
    ethHeader->h_dest = inet_addr(currentDestIp.c_str());
}

template <typename T>
void PacketSender<T>::createIpHeader(struct iphdr* ipHeader) {
    ipHeader->ihl = senderConfig_.IHL;
    ipHeader->version = senderConfig_.IP_VERSION;
    ipHeader->tos = senderConfig_.DEFAULT_TOS;
    ipHeader->tot_len = htons(totalPacketSize_);
    ipHeader->id = htons(packetID_++);
    ipHeader->frag_off = senderConfig_.FLAGS_FRAGMENT;
    ipHeader->ttl = senderConfig_.DEFAULT_TTL;
    ipHeader->protocol = senderConfig_.PROTOCOL_TYPE_INDEX;
    ipHeader->check = senderConfig_.INITIAL_IP_CHECKSUM;
    ipHeader->saddr = inet_addr(ethDevice_.getSelfIp().c_str());
    ipHeader->daddr = inet_addr(currentDestIp.c_str());

    // Compute checksum for the IP header
    ipHeader->check = computeChecksum(reinterpret_cast<uint8_t*>(ipHeader), senderConfig_.IPV4_HEADER_SIZE);
}

template <typename T>
void PacketSender<T>::assemblePacket() {
    totalPacketSize_ = payloadBuffer_.size() + senderConfig_.IPV4_HEADER_SIZE + senderConfig_.UDP_HEADER_SIZE;
    packetBuffer_.resize(totalPacketSize_); // Resize packet buffer to total size

    // Copy IP header
    iphdr* ipHeader = reinterpret_cast<iphdr*>(packetBuffer_.data());
    createIpHeader(ipHeader);

    // Copy UDP header
    udphdr* udpHeader = reinterpret_cast<udphdr*>(packetBuffer_.data() + sizeof(iphdr));
    createProtocolHeader(udpHeader);

   std::memcpy(packetBuffer_.data() + sizeof(iphdr) + sizeof(udphdr), payloadBuffer_.data(), payloadBuffer_.size());
}

template <typename T>
void PacketSender<T>::sendPacket() {
    if (packetBuffer_.empty()) {
        Logger::getInstance().Log(LogLevel::ERROR, "[PacketSend] Error: No packet assembled. Call assemblePacket() first.");
        return;
    }

    struct sockaddr_in destAddr{};
    destAddr.sin_family = AF_INET;
    destAddr.sin_port = htons(currentDestPort);
    destAddr.sin_addr.s_addr = inet_addr(currentDestIp.c_str());
    
    if (sendto(ethDevice_.getSocketFd(), packetBuffer_.data(), packetBuffer_.size(), 0,
            (struct sockaddr*)&destAddr, sizeof(destAddr)) < 0) {
        Logger::getInstance().Log(LogLevel::ERROR, "[PacketSend] Failed to send packet from ip " + ethDevice_.getSelfIp() + ":" + std::to_string(currentSrcPort)  
                                                    + " to ip " + currentDestIp + ":" + std::to_string(currentDestPort));
    }
    packetBuffer_.clear();
}

template <typename T>
EthDevice PacketSender<T>::extract_device(std::vector<T> buffer) {
    int device_index = (buffer[ICD_INDEX] & CONVENTION_MASK) >> CONVENTION_OFFSET;
    return NetworkManager::getDeviceByRole(EthDevice::Role(device_index));
}


template <typename T>
uint16_t PacketSender<T>::computeChecksum(uint8_t* data, int len) {
    uint32_t sum = 0;
    while (len > 1) {
        sum += *(data++);
        sum += (*(data++)) << 8;
        len -= 2;
    }
    if (len)
        sum += *data;
    while (sum >> 16)
        sum = (sum & 0xFFFF) + (sum >> 16);
    return static_cast<uint16_t>(~sum);
}


//This is the ICD:
/*
struct ICD_BIT_BUILD icd;
    icd.EOP = (ICD & 128) >> 7;
    icd.IS_COMP = (ICD & 64) >> 6;
    icd.CONV = (ICD & 48) >> 4;
    icd.SEQ_NUM = (ICD & 12) >> 2;
    icd.MSG_ID = (ICD & 3);
    return icd;

*/