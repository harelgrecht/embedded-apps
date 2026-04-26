#include "modules/GPIOHandler/GPIOHandler.hpp"
#include "modules/Logger/Logger.hpp"
#include "modules/NetworkManager/NetworkManager.hpp"
#include "modules/PacketSender/PacketSender.hpp"
#include "modules/PacketAuthentication/PacketAuthentication.hpp"
#include "modules/PacketRouter/PacketRouter.hpp"
#include "modules/RealTimeThread/RealTimeThread.hpp"
#include "modules/third_party/nlohmann/json.hpp"
#include "modules/ThreadSafeQueue/ThreadSafeQueue.hpp"
#include "modules/TunnelReceiverProcess/TunnelReceiverProcess.hpp"
#include "modules/TunnelTransmitterProcess/TunnelTransmitterProcess.hpp"
#include "modules/EthernetCapture/EthernetCapture.hpp"
#include "modules/InterfaceStatus/InterfaceStatus.hpp"

int main() {
    Logger::getInstance("MustLog.txt").Log(LogLevel::INFO, "Start Must...");
    GPIOHandler::loadConfig();
    
    ThreadSafeQueue<char> recvQueue;
    ThreadSafeQueue<char> fromTunnelQueue;
    ThreadSafeQueue<char> sendQueue;

    InterfaceStatus interfaceStatus;
    interfaceStatus.StartUpLeds();

    NetworkManager networkManager;
    networkManager.parseSettings("user_setting.json", "self_search_ips.json");
    networkManager.applySettings();

    EthernetCapture<char> capture(recvQueue, networkManager.getDeviceByRole(EthDevice::Role::TUNNEL));
    PacketRouter<char> router(recvQueue, fromTunnelQueue);
    TunnelReceiverProcess<char> receiverProcess(sendQueue, fromTunnelQueue);
    PacketSender<char> sender(sendQueue);

    RealTimeThread capture_thread([&](void*) {
        capture.startCapture();
    }, nullptr); 

    RealTimeThread router_thread([&](void*) {
        router.packetRouteHandler();
    }, nullptr);

    RealTimeThread recvProcess_thread([&](void*) {
        receiverProcess.processFromTunnelPackets();
    }, nullptr);

    RealTimeThread sender_thread([&](void*) {
        sender.runSender();
    }, nullptr);
    
    interfaceStatus.StartEthStatusMonitor();

    return 0;
}
