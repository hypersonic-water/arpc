/*
 * packet.c
 * Code to create ARP
 * and Ethernet packet
 */

#include "arpc.h"
#include "packet.h"

// Craft ARP payload and Ethernet frame
void craft_arp_payload(const header_info* h, char* result, const unsigned int target_ip) {
    unsigned char broadcast_addr[] = {0xff, 0xff, 0xff, 0xff, 0xff, 0xff}; // broadcast address -> ff:ff:ff:ff
    ETHERNET_FRAME* etherframe = (ETHERNET_FRAME*) result;

    // Set Ethernet header
    memcpy(etherframe->src_addr, h->macaddr, 6);
    memcpy(etherframe->dest_addr, broadcast_addr, 6);
    etherframe->type = htons(0x0806); // htons for 16 bit fields

    // Set ARP header
    etherframe->data.hardware_type = htons(1); // Ethernet
    etherframe->data.protocol_type = htons(0x800); // IPv4
    etherframe->data.hardware_length = 6;
    etherframe->data.protocol_length = 4;
    etherframe->data.opcode = htons(1);
    memcpy(etherframe->data.sender_hardware_addr, h->macaddr, 6);
    memset(etherframe->data.target_hardware_addr, 0x00, 6);
    etherframe->data.sender_protocol_addr = (h->ipaddr); // htonl for 32 bit protocol_addr field
    etherframe->data.target_protocol_addr = (target_ip);
}

// Decode arp response
int decode_arp_response(const char* resp, header_info* h, unsigned int expected_ip) {
    ETHERNET_FRAME* etherframe = (ETHERNET_FRAME*) resp;
    if (ntohs(etherframe->data.opcode) != 2)
        return 0;

    if (etherframe->data.sender_protocol_addr != expected_ip)
        return 0;

    memcpy(h->macaddr, etherframe->data.sender_hardware_addr, 6);
    h->ipaddr = etherframe->data.sender_protocol_addr;

    return 1;
}

