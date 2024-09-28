/*
    Program to tell serve the local time on the internet.
    AUTOHR: Nkono Ndeme Miguel
    COMPANY: Drean Technology
*/

// Headers for different platform.
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

#if defined(_WIN32)
#define ISVALIDSOCKET(s) ((s) != INVALID_SOCKET)
#define CLOSESOCKET(s) closesocket(s)
#define GETSOCKETERRNO() (WSAGetLastError())

#else
#define ISVALIDSOCKET(s) ((s) >= 0)
#define CLOSESOCKET(s) close(s)
#define SOCKET int
#define GETSOCKETERRNO() (errno)

#endif

// Includes
#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <string.h>

int main()
{
#if defined(_WIN32)
    WSADATA d;
    if (WSAStartup(MAKEWORD(2, 2), &d))
    {
        fprintf(stderr, "Failed to initialize to socket.\n");
        return 1;
    }
#endif
    /* figure out the local address. */

    printf("Configuring the local address...\n");
    struct addrinfo hints;
    memset(&hints, 0, sizeof(hints));
    hints.ai_family = AF_INET;       // IPv4
    hints.ai_socktype = SOCK_STREAM; // TCP (SOCK_DGRAM -> UDP)
    hints.ai_flags = AI_PASSIVE;    // wildcard address.

    struct addrinfo *bind_address;  // holds the return information from getaddrinfo()
    getaddrinfo(0, "8080", &hints, &bind_address);  // generate an address suitable to bind.

    /* create the socket. */
    printf("Create the socket...\n");
    SOCKET socket_listen;
    socket_listen = socket(bind_address->ai_family, bind_address->ai_socktype, bind_address->ai_protocol);  // create the socket.
    if (!ISVALIDSOCKET(socket_listen)) {
        fprintf(stderr, "socket() failde. (%d)\n", GETSOCKETERRNO());
        exit(EXIT_FAILURE);
    }

    /* Bind the socket with the address from getaddrinfo. */
    printf("Binding socket to local address...\n");
    if (bind(socket_listen, bind_address->ai_addr, bind_address->ai_addrlen)) {
        fprintf(stderr, "bind() failed. (%d)\n", GETSOCKETERRNO());
        exit(EXIT_FAILURE);
    }
    freeaddrinfo(bind_address);

    /* Start listening for connections. */
    printf("Listening...\n");
    if (listen(socket_listen, 10) < 0) {
        fprintf(stderr, "listen() failed. (%d)\n", GETSOCKETERRNO());
        exit(EXIT_FAILURE);
    }

    /* Accepting connections from the outside world. */
    printf("Waiting for connection...\n");
    struct sockaddr_storage client_address;
    socklen_t client_len = sizeof(client_address);
    SOCKET socket_client = accept(socket_listen, (struct sockaddr *)&client_address, &client_len);
    if (!ISVALIDSOCKET(socket_client)) {
        fprintf(stderr, "accept() failed. (%d)\n", GETSOCKETERRNO());
        exit(EXIT_FAILURE);
    }

    /* printing the client's address to the terminal. */
    printf("Client is connected...");
    char address_buffer[100];
    getnameinfo((struct sockaddr *)&client_address, client_len, address_buffer, sizeof(address_buffer), 0, 0, NI_NUMERICHOST);
    printf("%s\n", address_buffer);

    /* receving the request from the client. */
    printf("Reading request...\n");
    char request[1024];
    int bytes_received = recv(socket_client, request, sizeof(request), 0);
    printf("Received %d bytes.\n", bytes_received);
    printf("%.*s", bytes_received, request);    // print the web browser's request to the console.

    // send the response back to the web browser.
    printf("Sending response...\n");
    const char *response =
        "HTTP/1.1 2020 OK\r\n"
        "Connection: close\r\n"
        "Content-Type: text/plain\r\n\r\n"
        "Local time is: ";
    int bytes_sent = send(socket_client, response, strlen(response), 0);
    printf("Sent %d of %d bytes.\n", bytes_sent, (int)strlen(response));

    // sent the local time.
    time_t timer;
    time(&timer);
    char *time_msg = ctime(&timer);
    bytes_sent = send(socket_client, time_msg, strlen(time_msg), 0);
    printf("Sent %d of %d bytes.\n", bytes_sent, (int)strlen(time_msg));

    // Closing the connection.
    printf("Closing the connection...\n");
    CLOSESOCKET(socket_client);

    // Closing the lisening socket.
    printf("Closing the listening socket...\n");
    CLOSESOCKET(socket_listen);

#if defined(_WIN32)
    WSACleanup();
#endif
    return EXIT_SUCCESS;
}

/*
int getaddrinfo
    (const char *node, const char *service, const struct addrinfo *hints,struct addrinfo **res);
*/