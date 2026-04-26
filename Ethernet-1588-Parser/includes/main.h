#ifndef MAIN_H
#define MAIN_H


#define ETHERTYPE_PTP 0x88F7
#define ETH_DEVICE "eth0"

#define ETHERTYPE_OFFSET 12 // ethertype are 2 bytes at the end of the header

#define CAPTURE_READ_TIMEOUT_MS 1
#define PACKET_CAPTURE_LOOP_COUNT -1
#define PROMISC 1

void packet_handler(char *user, const struct pcap_pkthdr *header, const char *packet);


#endif
