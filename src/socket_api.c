/*
    Socket is one endpoint of a communication link between systems.
    socket API header files diferes from Berkely socket to Winsock, and
    on Winsock we need to initialise and cleanup the API when we want to start and finish using it.
*/

// On Winsock, a socket is representate by SOCKET (typedef  unsigned int) and on UNIX is simply am [int].
#if !defined(_WIN32)
#define SOCKET int
#endif

// Check failure of the function socket().
#if defined(_WIN32)
#define ISVALIDSOCKET(s)    ((s) != INVALID_SOCKET)
#else 
#define ISVALIDSOCKET(s)    ((s) >= 0)
#endif

// Closing socket
#if defined(_WIN32)
#define CLOSESOCKET(s)  closesocket(s)
#else 
#define CLOSESOCKET(s)  close(s)
#endif

// Error handling.
#if defined(_WIN32)
#define GETSOCKETERROR()    (WSAGetLastError())
#else
#define GETSOCKETERROR()    (errno)
#endif

#if defined(_WIN32)
#ifndef _WIN32_WINNT
#define _WIN32_WINNT 0x0600
#endif
#include <winsock2.h>
#include <ws2tcpip.h>
#pragma comment(lib, "ws2_32.lib")

#else
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <unistd.h>
#include <errno.h>

#endif

#include <stdio.h>

int main()
{
#if defined(_WIN32)
    WSADATA d;
    if (WSAStartup(MAKEWORD(2, 2), &d))
    {
        fprintf(stderr, "Failed to initialize.\n");
        return 1;
    }
#endif

    printf("Ready to use socket API.\n");

#if defined(_WIN32)
    WSACleanup();
#endif
    return 0;
}