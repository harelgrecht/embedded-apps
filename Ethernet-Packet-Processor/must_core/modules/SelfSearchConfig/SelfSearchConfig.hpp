#pragma once

#include <string>
#include "../third_party/nlohmann/json.hpp"

class SelfSearchConfig {
public:
    std::string selfIp_;         // The IP to configure on the device
    std::string ipToPing_;       // The IP to ping for connectivity check
    std::vector<std::string> destIpAddresses_;
    std::string defaultGateway_;
    std::string subnetMask_;
    std::string remoteIp_;
    std::string remoteIpDestination_;
    std::string role_;           // Role as a string (e.g., "source1")
    std::vector<int> srcPort_;
    std::vector<int> destPort_;

    // JSON deserialization for SelfSearchConfig
    friend void from_json(const nlohmann::json& j, SelfSearchConfig& config);
};

