#include "InterfaceStatus.hpp"

InterfaceStatus::InterfaceStatus() {
    interfaceMap_.reserve(ETH_COUNT);
    previousRxBytes_.fill(0);
    previousTxBytes_.fill(0);
    parseJson();
}

InterfaceStatus::~InterfaceStatus() {
    if(statusThread_.joinable())
        statusThread_.join();
}


void InterfaceStatus::StartEthStatusMonitor() {
    statusThread_ = std::thread(&InterfaceStatus::updateEthStatus, this);
}

void InterfaceStatus::updateEthStatus() {
    ethDevices_ = NetworkManager::getEthDevices();
    while (true) {
        for (size_t i = 0; i < ETH_COUNT; ++i) {
            const auto& device = ethDevices_[i];
            bool cableConnected = device.isCableConnected();

            if (cableConnected) {
                interfaceMap_[i].yellowLed.setHigh();
                size_t currentRxBytes = getRxBytes(interfaceMap_[i].name);
                size_t currentTxBytes = getTxBytes(interfaceMap_[i].name);
                if ((currentRxBytes > previousRxBytes_[i]) || (currentTxBytes > previousTxBytes_[i])) {
                    interfaceMap_[i].greenLed.blinkLed();
                }
                previousRxBytes_[i] = currentRxBytes;
                previousTxBytes_[i] = currentTxBytes;
            } else {
                interfaceMap_[i].yellowLed.setLow();
                interfaceMap_[i].greenLed.setLow(); // Ensure green LED is off if cable is disconnected
            }
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }
}

void InterfaceStatus::StartUpLeds() {
    const int repeatCount = 2;
    for (int cycle = 0; cycle < repeatCount; ++cycle) {
        // Turn on LEDs from left to right: yellow then green for each interface.
        for (auto& entry : interfaceMap_) {
            entry.yellowLed.setHigh();
            std::this_thread::sleep_for(std::chrono::milliseconds(200));
            entry.greenLed.setHigh();
            std::this_thread::sleep_for(std::chrono::milliseconds(200));
        }
        // Turn off LEDs from right to left: yellow then green for each interface.
        for (auto it = interfaceMap_.rbegin(); it != interfaceMap_.rend(); ++it) {
            it->greenLed.setLow();
            std::this_thread::sleep_for(std::chrono::milliseconds(200));
            it->yellowLed.setLow();
            std::this_thread::sleep_for(std::chrono::milliseconds(200));
        }
    }
}

size_t InterfaceStatus::getRxBytes(const std::string& interfaceName) {
    std::string path = "/sys/class/net/" + interfaceName + "/statistics/rx_bytes";
    std::ifstream file(path);
    if (!file.is_open()) {
        Logger::getInstance().Log(LogLevel::ERROR, "unalbe to read file: " + path);
        return 0;
    }
    size_t rxBytes = 0;
    file >> rxBytes;
    return rxBytes;
}

size_t InterfaceStatus::getTxBytes(const std::string& interfaceName) {
    std::string path = "/sys/class/net/" + interfaceName + "/statistics/tx_bytes";
    std::ifstream file(path);
    if (!file.is_open()) {
        Logger::getInstance().Log(LogLevel::ERROR, "unalbe to read file: " + path);
        return 0;
    }
    size_t txBytes = 0;
    file >> txBytes;
    return txBytes;
}

void InterfaceStatus::parseJson() {
    std::ifstream inputSettings(SETTING_FILE_);

    // Check if the file could be opened.
    if (!inputSettings.is_open()) {
        std::cerr << "Failed to open file: " << SETTING_FILE_ << '\n';
        return;
    }

    // Use Nlohmann JSON library to parse the JSON data from the stream.
    nlohmann::json jsonConfig;
    inputSettings >> jsonConfig;

    for(int index = 0; index < ETH_COUNT; index++) {
    //    interfaceMap_[index] = InterfaceMapEntry(jsonConfig[index]["name"], jsonConfig[index]["yellow_pin"], jsonConfig[index]["green_pin"]);
        
        interfaceMap_.emplace_back(jsonConfig[index]["name"], jsonConfig[index]["yellow_pin"], jsonConfig[index]["green_pin"]);
    }
}
