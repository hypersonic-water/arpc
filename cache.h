#ifndef CACHE_H
#define CACHE_H

void display_arp_table(void);
void get_macaddr_from_ipaddr(const char* iface, header_info* h, const int* socket_fd);

#endif //CACHE_H
