#include "csapp.h"

int main()
{
    int x = 1;

    if(Fork() == 0)
        printf("p1: %d\n", ++x);
    printf("p2: %d\n", --x);

    return 0;
}