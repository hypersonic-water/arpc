/*
 * iface.c
 * Code to get mac address and ip address
 * from a given interface
 */

#include "arpc.h"

void get_iface_info(const char*  iface, header_info* hddr_info) {
    int sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    if (sockfd == -1) {
        perror("arpc");
        exit(EXIT_FAILURE);
    }
    struct ifreq ifr;

    // Get MAC address
    strncpy(ifr.ifr_name, iface, IFNAMSIZ-1);
    ifr.ifr_name[IFNAMSIZ-1] = '\0';

    if (ioctl(sockfd, SIOCGIFHWADDR, &ifr) == -1) {
        fprintf(stderr, "arpc: No such interface %s\n", ifr.ifr_name);
        close(sockfd);
        exit(EXIT_FAILURE);
    }

    memcpy(hddr_info->macaddr, ifr.ifr_hwaddr.sa_data, 6);

    // Get IP address
    if (ioctl(sockfd, SIOCGIFADDR, &ifr) == -1) {
        fprintf(stderr, "arpc: No IP Address found for interface %s\n", ifr.ifr_name);
        close(sockfd);
        exit(EXIT_FAILURE);
    }

    const struct sockaddr_in *s = (struct sockaddr_in*) &ifr.ifr_addr;
    hddr_info->ipaddr = s->sin_addr.s_addr;

    close(sockfd);
}
