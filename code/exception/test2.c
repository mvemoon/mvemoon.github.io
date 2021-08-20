#include "csapp.h"

///虚拟地址比较


int main()
{
    int x = 1;

    if(Fork() == 0)
        printf("p1: %p\n", &x);
    printf("p2: %p\n", &x);

    return 0;
}