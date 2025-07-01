cmake_minimum_required(VERSION 3.31)
project(arpc C)

set(CMAKE_C_STANDARD 11)

add_executable(arpc main.c
        iface.c
        iface.h
        arpc.h
        packet.c
        packet.h
        arpc.c
        cache.c
        cache.h
        argsparse.c
        argsparse.h)
