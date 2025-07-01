/*
 * cache.c
 * Code to read
 * local ARP Cache
 */

#include "arpc.h"

// Display ARP Table located at /proc/net/arp
void display_arp_table(void) {
    FILE* f = fopen("/proc/net/arp", "r");
    if (!f) terminate(NULL);
    char header[1024];
    while (fgets(header, 1024, f) != NULL) {
        printf("%s", header);
    }
}


// Get MAC Addr for a given IP Addr
void get_macaddr_from_ipaddr(const char* iface,  header_info* h, const int* socket_fd) {
    int sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd == -1) terminate(socket_fd);

    struct arpreq req = {0};

    // Fill in protocol address (IP)
    struct sockaddr_in* s = (struct sockaddr_in*)&req.arp_pa;
    s->sin_addr.s_addr = h->ipaddr;
    s->sin_family = AF_INET;

    // Set interface name
    strncpy(req.arp_dev, iface, IFNAMSIZ - 1);
    req.arp_dev[IFNAMSIZ-1] = '\0';



    // Resolve Mac Address using ioctl
    if (ioctl(sockfd, SIOCGARP, &req) == -1) {
        memset(h->macaddr, 0, 6); // Interface not found
        return;
    }
    close(sockfd);

    if (!(req.arp_flags & ATF_COM)) {
        memset(h->macaddr, 0, 6); // MAC Address not found
        return;
    }

    // Copy MAC Address into structure
    memcpy(h->macaddr, req.arp_ha.sa_data, 6);
}