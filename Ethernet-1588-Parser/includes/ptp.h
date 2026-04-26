#ifndef PTP_H
#define PTP_H

#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <arpa/inet.h>
#include <time.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/mman.h>

#define PTP_TIMESTAMP_OFFSET 34
#define ETHERNET_HEADER_SIZE 14 // 6bytes(macDest) + 6bytes(macSrc) + 2bytes(ethertype) - layer2 ethernet
#define ORIGIN_TIMESTAMP_SECONDS_LENGTH 6
#define ORIGIN_TIMESTAMP_NANOSECONDS_LENGTH 4
#define SECONDS_PER_HOUR 3600
#define HOURS_PER_DAY 24
#define MINUTES_PER_HOUR 60
#define SECONDS_PER_MINUTE 60
#define GMT_ISRAEL_OFFSET 2
#define SECONDS_PER_DAY 86400
#define SYNC_MESSAGE_ID 0x08
#define FOLLOW_UP_MESSAGE_ID 0x00
#define BRAM_PHYSICAL_ADDRESS 0xA0020000
#define BRAM_SIZE 0x8000
#define BRAM_PATH "/dev/mem"


typedef struct ptpv2_packet {
    uint8_t transport_specific_message_type;
    uint8_t version;
    uint8_t message_length[2];
    uint8_t domain_number;
    uint8_t reserved1;
    uint8_t flags[2];
    uint8_t correction_field[8];
    uint8_t reserved2[4];
    uint8_t source_port_identity[10];
    uint8_t sequence_id[2];
    uint8_t control_field;
    uint8_t log_message_interval;
    uint8_t origin_timestamp_seconds[6];
    uint8_t origin_timestamp_nanoseconds[4];
} ptpv2_packet_t;

void parse_ptpv2_packet(const char *packet);
void convert_to_tod(uint64_t seconds, uint32_t nanoseconds);
uint64_t extract_origin_seconds(const struct ptpv2_packet* ptp_packet);
uint32_t extract_origin_nanoseconds(const struct ptpv2_packet* ptp_packet);
void write_to_bram(off_t bram_addr, unsigned int bram_size, uint64_t seconds, uint32_t nanoseconds);


#endif // PTP_H





