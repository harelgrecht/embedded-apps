#include <pcap.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "../inc/main.h"
#include "../inc/ptp.h"


int main() {

    char errbuf[PCAP_ERRBUF_SIZE];
    pcap_t *handle = pcap_open_live(ETH_DEVICE, BUFSIZ, PROMISC, CAPTURE_READ_TIMEOUT_MS, errbuf);
    if (!handle) {
        fprintf(stderr, "Error opening device: %s\n", errbuf);
        return EXIT_FAILURE;
    }

    printf("Listening for PTPv2 packets on eth0...\n");

    pcap_loop(handle, PACKET_CAPTURE_LOOP_COUNT, packet_handler, NULL);

    pcap_close(handle);
    return EXIT_SUCCESS;
}

void packet_handler(char *user, const struct pcap_pkthdr *header, const char *packet) {
    (void)user;
    // clock_t start = clock(); // Start the timer
    const uint16_t ethertype = ntohs(*(uint16_t *)(packet + ETHERTYPE_OFFSET));
    if (ethertype == ETHERTYPE_PTP) {
        parse_ptpv2_packet(packet);
    }
    // clock_t end = clock(); // End the timer
    // double cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC; // Calculate elapsed time in seconds

    // printf("Packet handling time: %f seconds\n", cpu_time_used); // Print out the elapsed time
}
