/*
    For proper windows headers to be include.
    It should be include firstly in the program !!!
*/

#ifndef _WIN32_WINNT
    #define _WIN32_WINNT 0x0600
#endif

#include <winsock2.h>   // winsock api
#include <iphlpapi.h>   // winsock api
#include <ws2tcpip.h>   // winsock api

#include <stdio.h>      // printf
#include <stdlib.h>     // malloc, realloc, etc.

/*
    Tell Microsoft Visual C which libraries to link with the executable.
    If using MinGW, no need to include those lines, rather add [-liphlpapi -lws2_32] flags
    to the compilation program.
*/

#pragma comment(lib, "ws2_32.lib")
#pragma comment(lib, "iphlpapi.lib")

// entry point.
int main() {
    WSADATA d;

    if (WSAStartup(MAKEWORD(2, 2), &d)) {
        fprintf(stderr, "Failed to load.\n");
        return EXIT_FAILURE;
    }

    // TODO -> implement the rest on windows machine.

    WSACleanup();

    return EXIT_SUCCESS;
}