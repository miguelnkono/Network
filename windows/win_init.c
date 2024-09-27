// windows networking api is call [Winsock].
// Whenever we are using the winsock api, we must first initialize it with the call to WSAStratup().
// And when we're done, we must close it with the call to WSACleanup().

/*
* Here is small program showing the initialization and the clean up of the Winsock.
*/

#include <stdio.h>
#include <stdlib.h>
#include <winsock2.h>
#pragma comment(lib, "ws2_32.lib")

int main() {
    WSADATA d;

    /*
        WSAStartup() is called with the requested version, in this case : version 2.2 [MAKEWORD(2, 2)].
        And a WSADATA structure. The WSADATA will be filled with details of the windows sockets 
        implementation. The WSAStartup() returns [0] upon sucess and non-zero upon failure.
    */
    if(WSAStartup(MAKEWORD(2, 2), &d)) {
        fprintf(stderr, "Failed to initialize.\n");
        return EXIT_FAILURE; // -1.
    }

    WSACleanup();
    printf("Ok.\n");

    return EXIT_SUCCESS;
}