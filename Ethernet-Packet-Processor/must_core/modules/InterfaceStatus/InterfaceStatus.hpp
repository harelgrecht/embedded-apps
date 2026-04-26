#pragma once

#include <iostream>
#include <string> 
#include <array>
#include <thread>
#include <atomic>
#include <sstream>
#include <fstream>
#include <chrono>
#include "../GPIOHandler/GPIOHandler.hpp"
#include "../Logger/Logger.hpp"
#include "../third_party/nlohmann/json.hpp"
#include "../NetworkManager/NetworkManager.hpp"

const int ETH_COUNT = 4;

struct InterfaceMapEntry {
    std::string name;
    GPIOHandler greenLed;
    GPIOHandler yellowLed;

    InterfaceMapEntry(const std::string& interface, int greenPin, int yellowPin) 
        : name(interface), greenLed(greenPin, "out"), yellowLed(yellowPin, "out") { }

    InterfaceMapEntry() : name(), greenLed(0, "out"), yellowLed(0, "out") { }

    InterfaceMapEntry(const InterfaceMapEntry& other) = delete;

    InterfaceMapEntry(InterfaceMapEntry&& other) noexcept
    : name(std::move(other.name)), greenLed(std::move(other.greenLed)), yellowLed(std::move(other.yellowLed)) {
    }

    ~InterfaceMapEntry() {}
};


class InterfaceStatus {
    public:
        InterfaceStatus();
        ~InterfaceStatus();
        
        void StartEthStatusMonitor();
        void StartUpLeds();


    private:
        const char* SETTING_FILE_ = "indicator_leds_settings.json";

        // std::array<InterfaceMapEntry, ETH_COUNT> interfaceMap_;
        std::vector<InterfaceMapEntry> interfaceMap_;
        std::array<size_t, ETH_COUNT> previousRxBytes_;
        std::array<size_t, ETH_COUNT> previousTxBytes_;

        std::array<EthDevice, ETH_COUNT> ethDevices_;
        
        std::thread statusThread_;

        size_t getRxBytes(const std::string& interfaceName);
        size_t getTxBytes(const std::string& interfaceName);
        void updateEthStatus();
        void parseJson();
};