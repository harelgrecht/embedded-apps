#include "../inc/ptp.h"
#include "../inc/main.h"
#include <stdio.h>
#include <string.h>
#include <arpa/inet.h>
#include <time.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/mman.h>

typedef int u32;

void parse_ptpv2_packet(const char *packet) {
    const ptpv2_packet_t *ptp_packet = ( ptpv2_packet_t *)(packet + ETHERNET_HEADER_SIZE);

    uint8_t message_type = ptp_packet->transport_specific_message_type & 0x0F;
    uint64_t timestamp_seconds = extract_origin_seconds(ptp_packet);
    uint32_t timestamp_nanoseconds = extract_origin_nanoseconds(ptp_packet);

    if(message_type == SYNC_MESSAGE_ID || message_type == FOLLOW_UP_MESSAGE_ID) {
        if (timestamp_seconds != 0 || timestamp_nanoseconds != 0) {
            //convert_to_tod(timestamp_seconds, timestamp_nanoseconds);
        	write_to_bram(BRAM_PHYSICAL_ADDRESS, BRAM_SIZE, timestamp_seconds, timestamp_nanoseconds);
        }
    }
}

void convert_to_tod(uint64_t seconds, uint32_t nanoseconds) {
    uint64_t total_seconds = seconds % SECONDS_PER_DAY;
    uint64_t hours = (total_seconds / SECONDS_PER_HOUR) + GMT_ISRAEL_OFFSET;
    uint64_t minutes = (total_seconds % SECONDS_PER_HOUR) / MINUTES_PER_HOUR;
    uint64_t secs = total_seconds % SECONDS_PER_MINUTE;
    printf("Time of Day (ToD): %02lu:%02lu:%02lu.%09u\n", hours, minutes, secs, nanoseconds);
}


uint64_t extract_origin_seconds(const struct ptpv2_packet* ptp_packet) {
    uint64_t seconds = 0;
    for(int i = 0; i < ORIGIN_TIMESTAMP_SECONDS_LENGTH; i++) {
        seconds = (seconds << 8) | ptp_packet->origin_timestamp_seconds[i];
    }
    return seconds;
}

uint32_t extract_origin_nanoseconds(const struct ptpv2_packet* ptp_packet) {
    uint32_t nanoseconds = 0;
    for(int i = 0; i < ORIGIN_TIMESTAMP_NANOSECONDS_LENGTH; i++) {
        nanoseconds = (nanoseconds << 8) | ptp_packet->origin_timestamp_nanoseconds[i];
    }
    return nanoseconds;
}


void write_to_bram(off_t bram_addr, unsigned int bram_size, uint64_t seconds, uint32_t nanoseconds) {
	int fd;
	u32 *bram32_sec_address;
	u32 *bram32_nanosec_address;
	if ((fd = open(BRAM_PATH, O_RDWR | O_SYNC)) != -1) {
		bram32_sec_address = (u32 *)mmap(NULL, bram_size, PROT_WRITE, MAP_SHARED, fd, bram_addr);
		bram32_nanosec_address = (u32 *)mmap(NULL, bram_size, PROT_WRITE, MAP_SHARED, fd, (bram_addr + 4));
		bram32_sec_address = seconds;
		bram32_nanosec_address = nanoseconds;
		printf("address 0 data: 0x%X and the actual address: 0x%x\n", seconds,  &bram32_sec_address);
		printf("address 4 data: 0x%X and the actual address: 0x%x\n", nanoseconds, &bram32_nanosec_address);
		close(fd);
	}
}
