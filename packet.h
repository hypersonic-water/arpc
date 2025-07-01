#ifndef PACKET_H
#define PACKET_H


// ARP packet
typedef struct __attribute__((packed)) { // disallow padding to keep ARP HEADER 28 bytes
    unsigned short hardware_type;
    unsigned short protocol_type;
    unsigned char hardware_length;
    unsigned char protocol_length;
    unsigned short opcode; // 1 for arp request
    unsigned char sender_hardware_addr[6];
    unsigned int sender_protocol_addr;
    unsigned char target_hardware_addr[6]; // all 0s for arp request
    unsigned int target_protocol_addr;
} ARP_HEADER;

// Ethernet frame
typedef struct __attribute__((packed)){ // disallow padding to preserve ethernet frame structure
    unsigned char dest_addr[6];
    unsigned char src_addr[6];
    unsigned short type;
    ARP_HEADER data;
} ETHERNET_FRAME;

void craft_arp_payload(const header_info* h, char* result, const unsigned int target_ip);
int decode_arp_response(const char* resp, header_info* h, unsigned int expected_ip);

#endif //PACKET_H
