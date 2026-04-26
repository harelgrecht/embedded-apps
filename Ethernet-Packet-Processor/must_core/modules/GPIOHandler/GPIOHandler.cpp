#include "GPIOHandler.hpp"

GPIOSettings GPIOHandler::config_;
std::string GPIOHandler::configFilePath_ = "GPIO_settings.json";

void GPIOHandler::setConfigPath(const std::string& configPath) {
    if (configFilePath_.empty()) {
        configFilePath_ = configPath;
        loadConfig();
    } else {
       Logger::getInstance().Log(LogLevel::ERROR, "Configuration path is already set.");
    }
}

void GPIOHandler::loadConfig() {
    std::ifstream configFile(configFilePath_);
    if (!configFile.is_open()) {
       Logger::getInstance().Log(LogLevel::ERROR, "Failed to open config file: " + configFilePath_);
        return;
    }
    try {
        parseJson();
    } catch (json::parse_error& e) {
       Logger::getInstance().Log(LogLevel::ERROR, "JSON parse error: " + std::string(e.what()));
        return;
    }
}

GPIOHandler::GPIOHandler(int ledPin, const std::string& direction) : pinNumber_(ledPin) {
    if (ledPin == 0) return;
    if (!writeToFile(config_.GPIO_EXPORT_PATH, std::to_string(pinNumber_))) {
       Logger::getInstance().Log(LogLevel::ERROR, "Error: Failed to export GPIO pin " + std::to_string(pinNumber_));
    }

    // Set GPIO direction
    if (!writeToFile(formatPath(config_.GPIO_DIRECTION_PATH), direction)) {
       Logger::getInstance().Log(LogLevel::ERROR, "Error: Failed to set direction for GPIO pin " + std::to_string(pinNumber_));
    }
}

GPIOHandler::~GPIOHandler() {
 //   if (!writeToFile(config_.GPIO_UNEXPORT_PATH, std::to_string(pinNumber_))) {
       Logger::getInstance().Log(LogLevel::ERROR, "Warning: Failed to unexport GPIO pin " + std::to_string(pinNumber_));
 //   }
}

bool GPIOHandler::setHigh() {
    return writeToFile(formatPath(config_.GPIO_VALUE_PATH), std::to_string(config_.GPIO_ON));
}

bool GPIOHandler::setLow() {
    return writeToFile(formatPath(config_.GPIO_VALUE_PATH), std::to_string(config_.GPIO_OFF));
}

bool GPIOHandler::blinkLed() {
    if (!writeToFile(formatPath(config_.GPIO_VALUE_PATH), std::to_string(config_.GPIO_ON))) {
        return false;
    }
    std::this_thread::sleep_for(std::chrono::milliseconds(500));
    return writeToFile(formatPath(config_.GPIO_VALUE_PATH), std::to_string(config_.GPIO_OFF));
}

std::string GPIOHandler::formatPath(const std::string pathTemplate) const {
    char formattedPath[100];
    snprintf(formattedPath, sizeof(formattedPath), pathTemplate.c_str(), pinNumber_);
    return std::string(formattedPath);
}

bool GPIOHandler::writeToFile(const std::string& filePath, const std::string& data) {
    std::ofstream file(filePath, std::ios_base::app);
    if (!file.is_open()) {
       Logger::getInstance().Log(LogLevel::ERROR, "Failed to open: " + filePath);
        return false;
    }
    file << data;
    if (file.fail()) {
       Logger::getInstance().Log(LogLevel::ERROR, "Failed to write to: " + filePath);
        return false;
    }
    file.close();
    return true;
}

void GPIOHandler::parseJson() {
    std::ifstream inputSettings(configFilePath_);

    // Check if the file could be opened.
    if (!inputSettings.is_open()) {
        std::cerr << "Failed to open file: " << configFilePath_ << '\n';
        return;
    }

    // Use Nlohmann JSON library to parse the JSON data from the stream.
    nlohmann::json jsonConfig;
    inputSettings >> jsonConfig;

    config_.GPIO_ON = jsonConfig["GPIO_ON"];
    config_.GPIO_OFF = jsonConfig["GPIO_OFF"];
    config_.GPIO_EXPORT_PATH = jsonConfig["GPIO_EXPORT_PATH"];
    config_.GPIO_UNEXPORT_PATH = jsonConfig["GPIO_UNEXPORT_PATH"];
    config_.GPIO_DIRECTION_PATH = jsonConfig["GPIO_DIRECTION_PATH"];
    config_.GPIO_VALUE_PATH = jsonConfig["GPIO_VALUE_PATH"];
}