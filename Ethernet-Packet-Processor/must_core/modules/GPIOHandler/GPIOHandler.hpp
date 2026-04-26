#pragma once

#include <iostream>
#include <fstream>
#include <string>
#include <cstdio>
#include <thread>
#include <chrono>
#include "../third_party/nlohmann/json.hpp"
#include "../Logger/Logger.hpp"

using json = nlohmann::json;

struct GPIOSettings {
    int GPIO_ON;
    int GPIO_OFF;
    std::string GPIO_EXPORT_PATH;
    std::string GPIO_UNEXPORT_PATH;
    std::string GPIO_DIRECTION_PATH;
    std::string GPIO_VALUE_PATH;
};

class GPIOHandler {
public:
    GPIOHandler(int ledPin, const std::string& direction);
    ~GPIOHandler();

    bool setHigh();
    bool setLow();
    bool blinkLed();
    int getPinNumber() const noexcept { return pinNumber_; }

    static void setConfigPath(const std::string& configPath);
    static void loadConfig();

private:
    bool writeToFile(const std::string& filePath, const std::string& data);
    std::string formatPath(const std::string pathTemplate) const;
    static void parseJson();

    int pinNumber_;
    static GPIOSettings config_;
    static std::string configFilePath_; // Static string to store the config file path

};
