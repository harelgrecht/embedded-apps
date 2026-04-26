#include "NetworkManager.hpp"

std::array<EthDevice, 4> NetworkManager::ethDevices_;

NetworkManager::NetworkManager() { }

// Parse the user settings file to get the normal EthDevice settings and the flag.
// If self-search is enabled, also load self-search configurations from the separate file.
void NetworkManager::parseSettings(const std::string& userSettingsFile, const std::string& selfSearchFile) {
    std::ifstream userFile(userSettingsFile);
    if (!userFile.is_open()) {
        throw std::runtime_error("Failed to open user settings file: " + userSettingsFile);
    }
    nlohmann::json userConfig;
    userFile >> userConfig;

    // Update each EthDevice with settings from user_setting.json
    for (int index = 0; index < ethDevices_.size(); index++) {
        std::string name = DEVICE_NAME_PREFIX + std::to_string(index);
        if (userConfig.contains(name)) {
            ethDevices_[index] = userConfig.at(name).get<EthDevice>();
            ethDevices_[index].initSocket();
        }
    }

    // Read the enableSelfSearch flag from the user settings.
    enableSelfSearch = userConfig.value("enableSelfSearch", false);

    if (enableSelfSearch) {
        // In self-search mode, load configurations from self_search_ips.json.
        std::ifstream searchFile(selfSearchFile);
        if (!searchFile.is_open()) {
            throw std::runtime_error("Failed to open self search file: " + selfSearchFile);
        }
        nlohmann::json searchConfig;
        searchFile >> searchConfig;
        if (searchConfig.is_array()) {
            selfSearchConfigs_ = searchConfig.get<std::vector<SelfSearchConfig>>();
        } else {
            throw std::runtime_error("Self search configuration is not an array.");
        }
    }
}

// In normal mode, apply settings for each EthDevice and set system routes.
// In self-search mode, run the self-search routine.
void NetworkManager::applySettings() {
    if (!enableSelfSearch) {
        Logger::getInstance().Log(LogLevel::INFO, "[NetworkManager] Normal mode: applying user settings.");
        for (auto& device : ethDevices_) {
            device.applySettings();
        }
        setSystemRoutes();
    } else {
        Logger::getInstance().Log(LogLevel::INFO, "[NetworkManager] Self search mode enabled. Running self-search routines...");
        runSelfSearchInParallel();
    }
}

void NetworkManager::setSystemRoutes() {
    Logger::getInstance().Log(LogLevel::INFO, "[NetworkManager] Setting system routes (normal mode placeholder).");
}

// For each Ethernet device, check cable connectivity and then run self-search configurations.
// If the cable is not detected, retry up to 5 times (over about 1 minute).
void NetworkManager::runSelfSearchForDevice(EthDevice& device, const std::vector<SelfSearchConfig>& configs) {
    const int maxAttempts = 5;
    int attempts = 0;
    while (!device.isCableConnected() && attempts < maxAttempts) {
        Logger::getInstance().Log(LogLevel::INFO, "[NetworkManager] Cable not connected on " + device.getName()
                  + ", attempt " + std::to_string(attempts + 1) + " of " + std::to_string(maxAttempts) + ". Retrying...");
        std::this_thread::sleep_for(std::chrono::seconds(12));
        attempts++;
    }
    if (!device.isCableConnected()) {
        Logger::getInstance().Log(LogLevel::INFO, "[NetworkManager] Cable still not connected on " + device.getName()
                  + " after " + std::to_string(maxAttempts) + " attempts. Skipping configuration.");

        return;
    }

    // Cable is connected; iterate over each self-search configuration.
    bool configApplied = false;
    for (const auto& config : configs) {
        device.applySelfSearchConfig(config);
        if (device.pingIP(config.ipToPing_)) {
            Logger::getInstance().Log(LogLevel::INFO, "[NetworkManager] Ping successful for config with ipToPing " + config.ipToPing_
                      + " on device " + device.getName() + ". Applying configuration.\n");
            configApplied = true;
            break;
        } else {
            Logger::getInstance().Log(LogLevel::INFO, "[NetworkManager] Ping failed for config with ipToPing " + config.ipToPing_
                      + " on device " + device.getName() + ". Trying next configuration.");
        }
    }
    if (!configApplied) {
        Logger::getInstance().Log(LogLevel::INFO, "[NetworkManager] No valid self-search configuration found for device " + device.getName());
    }
}

// Launch self-search for each Ethernet device in parallel.
void NetworkManager::runSelfSearchInParallel() {
    std::vector<std::thread> threads;
    for (auto& device : ethDevices_) {
        threads.emplace_back([this, &device]() {
            runSelfSearchForDevice(device, selfSearchConfigs_);
        });
    }
    for (auto& t : threads) {
        t.join();
    }
    Logger::getInstance().Log(LogLevel::INFO, "[NetworkManager] Self-search tasks completed for all devices.");
}

EthDevice NetworkManager::getDeviceByRole(EthDevice::Role role) {
    for (const EthDevice& device : ethDevices_) {
        if (device.getRole() == role) {
            return device;
        }
    }
}

std::array<EthDevice, 4>& NetworkManager::getEthDevices() {
    return ethDevices_;
}