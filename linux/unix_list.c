/*
    Listing local network addresses.
    author : Nkono Ndeme Miguel
    Date : 24-09-2024
*/

// headers
#include <sys/socket.h>
#include <netdb.h>
#include <ifaddrs.h>

#include <stdio.h>
#include <stdlib.h>

// entry point
int main()
{
    struct ifaddrs *addresses; // struct to strore the addressess.

    /*
        Create a linked list of the addresses on the host machie with `getifaddrs` function.
        If successful,store the list in `addresses` variable and return 0;
        On error, return -1 and set `errno`
    */
    if (getifaddrs(&addresses) != 0)
    {
        fprintf(stderr, "getifaddrs failded.\n");
        return EXIT_FAILURE;
    }

    struct ifaddrs *address = addresses; /* Create an address pointer to walk through the addressess.*/
    while (address != 0)
    {
        int family = address->ifa_addr->sa_family; /* get the family of tht address [ipv4 or ipv6]. */
        if (
            family == AF_INET     /* IPv4 */
            || family == AF_INET6 /* IPv6 */
        )
        {
            printf("%s\t", address->ifa_name);                     /* output the name of the network interface. */
            printf("%s\t", (family == AF_INET ? "IPv4" : "IPv6")); /* ouput the type of the interface .*/

            /* create a buffer `ap` to store the textual address. */
            char ap[1000];
            const int family_size =
                (family == AF_INET ? sizeof(struct sockaddr_in) : sizeof(struct sockaddr_in6));
            /* then call getnameinfo() to fill the buffer. */
            getnameinfo(
                address->ifa_addr,
                family_size,
                ap,
                sizeof(ap),
                0,
                0,
                NI_NUMERICHOST);
            printf("\t%s\n", ap);
        }

        /* advance to the next address. */
        address = address->ifa_next;
    }

    /* free the memorie allocated for the addresses. */
    freeifaddrs(addresses);

    return EXIT_SUCCESS;
}