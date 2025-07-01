/*
 * arpc.c
 * Code to send
 * ARP request
 */

#include "arpc.h"
#include "packet.h"
#include "cache.h"

int read_cache(const header_info* h, unsigned int target_ip, int sockfd, char* dest_ip);

// Display MAC Address
void print_hware_addr(const unsigned char* hwareaddr) {
    printf("%02x:%02x:%02x:%02x:%02x:%02x\n", hwareaddr[0],  hwareaddr[1], hwareaddr[2],  hwareaddr[3],  hwareaddr[4],  hwareaddr[5]);
}

// Send ARP Request
void arp_request(header_info* h, unsigned int target_ip, int no_cache) {
    unsigned char ether_frame[60] = {0}, response[1600] = {0};
    unsigned char broadcast_addr[] = {0xff, 0xff, 0xff, 0xff, 0xff, 0xff};
    craft_arp_payload(h, ether_frame, target_ip);

    int sockfd = socket(AF_PACKET, SOCK_RAW, htons(ETH_P_ARP));
    if (sockfd == -1) terminate(&sockfd);

    // Set sockaddr_ll
    struct sockaddr_ll dll = {0};
     dll.sll_family = AF_PACKET;
     dll.sll_ifindex = if_nametoindex(h->iface);
     dll.sll_halen    = ETH_ALEN;
    memcpy(dll.sll_addr, broadcast_addr, 6);

    char src_ip[INET_ADDRSTRLEN], dst_ip[INET_ADDRSTRLEN];



    inet_ntop(AF_INET, &target_ip, dst_ip, INET_ADDRSTRLEN);
    inet_ntop(AF_INET, &h->ipaddr, src_ip, INET_ADDRSTRLEN);

    printf("ARP SUMMARY\n");
    printf("ARP Request:\n    Source IP Address: %s\n    Target IP Address: %s\n\n", src_ip, dst_ip);

    if (!no_cache && read_cache(h, target_ip, sockfd, dst_ip)) {
        close(sockfd);
        return;
    };

    if (sendto(sockfd, ether_frame, 60, 0, (struct sockaddr*) &dll, sizeof(dll)) == -1) terminate(&sockfd);

    // Set timeout
    struct timeval timeout;
    timeout.tv_sec = 2;
    timeout.tv_usec = 0;
    if (setsockopt(sockfd, SOL_SOCKET, SO_RCVTIMEO, &timeout, sizeof(timeout)) < 0) {
        terminate(&sockfd);
    }

    // Reply must be from target
    while (1) {
        ssize_t len = recvfrom(sockfd, response, sizeof(response), 0, NULL, NULL);

        if (len < 0) {
            // If there is no reply after 2 seconds
            if (errno == EAGAIN || errno == EWOULDBLOCK) {
                fprintf(stderr, "No Reply from: %s\nRequest Timed Out!\n", dst_ip);
                close(sockfd);
                return;
            }
            terminate(&sockfd);
        }

        if (decode_arp_response(response, h, target_ip)) break;
    }


    printf("ARP Reply:\n    Sender IP Address: %s\n    Sender MAC Address: ", dst_ip);
    print_hware_addr(h->macaddr);
    printf("\n");

}

//
int read_cache(const header_info* h, unsigned int target_ip, int sockfd, char* dest_ip) {
    header_info cpy;
    cpy.ipaddr = target_ip;
    get_macaddr_from_ipaddr(h->iface,  &cpy, &sockfd);

    int i;
    for (i = 0; i<6; i++) {
        if (cpy.macaddr[i] != 0x00) break;
    }

    if (i != 6) {
        printf("ARP Reply (LOCAL CACHE)\n    Target IP Address: %s\n    Target MAC Address: ", dest_ip);
        print_hware_addr(cpy.macaddr);
        printf("\n");
        return 1;
    }

    return 0;

}

// Terminate the program
void terminate(const int* sockfd) {
    perror("arpc");
    if (sockfd != NULL) close(*sockfd);
    exit(EXIT_FAILURE);
}