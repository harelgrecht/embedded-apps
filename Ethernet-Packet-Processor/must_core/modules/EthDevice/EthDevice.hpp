#pragma once

#include <string>
#include <iostream>
#include <net/if.h>  // for IFF_UP
#include <iostream>
#include <chrono>
#include <sys/types.h>
#include <sys/socket.h>
#include <cstring>
#include <arpa/inet.h>
#include <unistd.h>
#include <netinet/ip_icmp.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <stdexcept>
#include <netinet/in.h>
#include <net/route.h>

#include "../third_party/nlohmann/json.hpp"
#include "../SelfSearchConfig/SelfSearchConfig.hpp" 
#include "../Logger/Logger.hpp"

class EthDevice {
    public:
        // Role for the device
        enum class Role { SOURCE1, SOURCE2, SOURCE3, TUNNEL };

        // Constructors
        explicit EthDevice(const std::string& name);
        EthDevice();

        // Destructor
        ~EthDevice();
        // Normal mode: apply the user-supplied settings
        void applySettings();

        // Check at hardware level if the cable is connected
        bool isCableConnected() const;

        // Get the device role
        const Role& getRole() const;

        // In self-search mode: update the device configuration from a SelfSearchConfig object
        void applySelfSearchConfig(const SelfSearchConfig& config);

        // JSON deserialization for normal (user) settings from user_setting.json
        friend void from_json(const nlohmann::json& j, EthDevice& ethDevice);

        // Pinging to given IP address.
        bool pingIP(const std::string& ip);

        std::string getSelfIp() const;

        std::vector<std::string> getDestIpList() const;

        bool isIpInDestList(const std::string& ip) const;

        std::vector<int> getSrcPortList() const;
        
        std::vector<int> getDestPortList() const;

        int getSocketFd() const;

        std::string getName() const;

        bool initSocket();
        
    private:
        std::string name_;
        std::string ipAddress_;
        std::vector<std::string> destIpAddresses_;
        std::string defaultGateway_;
        std::string subnetMask_;
        std::string remoteIp_;
        std::string remoteIpDestination_;
        std::vector<int> srcPort_;
        std::vector<int> destPort_;

        bool promisc_;
        bool noArp_;
        bool multicast_;
        bool debug_;
        bool dynamic_;
        bool notrailers_;
        bool broadcast_;
        int mtu_;
        Role role_;

        struct ifreq ifr;
        int sock_;
        int sockOptStatus;
        void closeSocket();

        uint16_t calculateChecksum(void* buf, int len);

        // Helper methods for applying settings
        void setDeviceFlags();
        void updateFlag(short& flags, unsigned int flag, bool enable);
        void setMTU();
        void addVirtualIpIfNeeded();
        void setSelfIP();
        void setDefaultGateway();
       
        // Helper: convert a string to Role enum
        static Role parseRole(const std::string& roleStr);
        std::string roleToString(const Role& role);
        std::string getInfoDestList();
        std::string getInfoSrcPortList();
        std::string getInfoDestPortList();

};

