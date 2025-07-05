# arpc — ARP Request Utility

A simple command-line tool written in C that sends ARP requests to determine the MAC address for a given IPv4 address on a specified network interface.

## Features

- Send ARP requests to lookup MAC addresses.
- Option to bypass local ARP cache (`--force` / `--no-cache`).
- Display the current ARP table.
- Supports specifying a network interface.
- Clean and concise output with option for detailed view.

---

## Usage

`arpc -i <interface> [options] <ip-address>`

## Options

| Flag                    | Description                                    |
| ----------------------- | ---------------------------------------------- |
| -i, --interface <iface> | Specify network interface (e.g., eth0, enp3s0) |
| -f, --force, --no-cache | Force ARP request (ignore local ARP cache)     |
| -t, --table             | Display the current ARP table (/proc/net/arp)  |
| -h, --help              | Show help message and exit                     |

## Examples

```
arpc -i enp3s0 192.168.1.1                # Send ARP request and check cache
arpc -i enp3s0 -f 192.168.1.1             # Force ARP request (ignore cache)
arpc --table                              # Display local ARP cache
