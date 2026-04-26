#include "SelfSearchConfig.hpp"

void from_json(const nlohmann::json& j, SelfSearchConfig& config) {
     j.at("selfIp").get_to(config.selfIp_);
    j.at("ipToPing").get_to(config.ipToPing_);
    j.at("destIpAddresses").get_to(config.destIpAddresses_);
    j.at("defaultGateway").get_to(config.defaultGateway_);
    j.at("subnetMask").get_to(config.subnetMask_);
    j.at("remoteIp").get_to(config.remoteIp_);
    j.at("remoteIpDestination").get_to(config.remoteIpDestination_);
    j.at("role").get_to(config.role_);
    j.at("srcPorts").get_to(config.srcPort_);
    j.at("destPorts").get_to(config.destPort_);
}
