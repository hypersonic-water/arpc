/*
 *  argsparse.c
 *  Code to parse
 *  command line arguments
 */

/*
 * Flags
 * -h help
 * -f force
 * -i interface
 * -t table
 */

#include "arpc.h"
#include "cache.h"
#include "iface.h"

void display_help() {
    printf("Usage: arpc -i <interface> [options] <ip-address>\n");
    printf("       arpc -t | --table\n\n");
    printf("Options:\n");
    printf("  -i, --interface <iface>   Specify network interface (e.g., eth0, enp3s0)\n");
    printf("  -f, --force, --no-cache   Force ARP request (do not check local ARP cache)\n");
    printf("  -t, --table               Display the current ARP table\n");
    printf("  -h, --help                Show this help message\n\n");
    printf("Examples:\n");
    printf("  arpc -i enp3s0 192.168.1.1           Send ARP request and check local cache\n");
    printf("  arpc -i enp3s0 -f 192.168.1.1        Force ARP request (ignore cache)\n");
    printf("  arpc --table                         Display local ARP cache\n");
}

unsigned int copy_target_ipaddr(char* ipaddr, const char* target) {
    strncpy(ipaddr, target, INET_ADDRSTRLEN);
    unsigned int target_ip;
    if (inet_pton(AF_INET, ipaddr, &target_ip) != 1) {
        printf("arpc: Incorrect or Invalid IP Address - %s\n", ipaddr);
        exit(EXIT_FAILURE);
    }

    return target_ip;
}

// parse command line arguments
void parseargs(int argc, char *argv[]) {
    char ipaddr[INET_ADDRSTRLEN];
    int force = 0, intf = 0, ip_specfd = 0;
    unsigned int target_ip;
    header_info h;

    if (argc == 1) {
        fprintf(stderr, "Usage: arpc -i <interface> [options] <ip-address>\n");
        fprintf(stderr, "Try 'arpc --help' for more information.\n");
        exit(EXIT_FAILURE);
    }

    if (strcmp(argv[1], "-h") == 0 || strcmp(argv[1], "--help") == 0) {
        display_help();
        exit(EXIT_SUCCESS);
    }


    if ((strcmp(argv[1], "-t") == 0 || strcmp(argv[1], "--table") == 0) && argc == 2) {
        display_arp_table();
        exit(EXIT_SUCCESS);
    }


    for (int i = 1; i<argc; i++) {
        if (strcmp(argv[i], "-i") == 0 || strcmp(argv[i], "--interface") == 0) {
            if (i+1 >= argc) {
                fprintf(stderr, "Incomplete arguments provided for - '%s'\n", argv[i]);
                display_help();
            }
            strncpy(h.iface, argv[i+1], IFNAMSIZ-1);
            h.iface[IFNAMSIZ-1] = '\0';
            intf = 1;
            i++;
        }
        else if (argv[i][0] != '-') {
            if (!ip_specfd) {
                target_ip = copy_target_ipaddr(ipaddr, argv[i]);
                ip_specfd = 1;
            }

            else {
                fprintf(stderr, "argc: Unknown flag - '%s' OR incorrect argument format\n", argv[i]);
                display_help();
                exit(EXIT_FAILURE);
            }

        }

        else if (strcmp(argv[i], "-f") == 0 || strcmp(argv[i], "--no-cache") == 0 || strcmp(argv[i], "--force") == 0) {
            force = 1;
        }

        else {
            fprintf(stderr, "argc: Unknown flag - '%s' OR incorrect argument format\n", argv[i]);
            display_help();
            exit(EXIT_FAILURE);
        }
    }

    if (!ip_specfd) {
        fprintf(stderr, "arpc: Target IP Address not provided\n");
        display_help();
        exit(EXIT_FAILURE);
    }

    if (!intf) {
        fprintf(stderr, "arpc: Interface value not provided\n");
        display_help();
        exit(EXIT_FAILURE);
    }



    get_iface_info(h.iface, &h);
    arp_request(&h, target_ip, force);

}
