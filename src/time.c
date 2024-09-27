#include <stdio.h>
#include <time.h>

int main() 
{
    time_t t;
    time(&t);

    printf("The local time is : %s\n", ctime(&t));
    return 0;
}