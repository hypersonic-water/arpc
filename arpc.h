#ifndef ARPC_H
#define ARPC_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <net/if.h>
#include <arpa/inet.h>
#include <errno.h>
#include <sys/socket.h>
#include <netpacket/packet.h>
#include <net/ethernet.h>
#include <net/if_arp.h>

typedef struct {
    unsigned char macaddr[6];
    unsigned int ipaddr;
    char iface[IFNAMSIZ];
} header_info;


void arp_request(header_info* h, unsigned int target_ip, int no_cache);
void terminate(const int* sockfd);

#endif //ARPC_H
