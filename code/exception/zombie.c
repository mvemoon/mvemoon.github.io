#include "csapp.h"

int main(void)
{
    if(fork() == 0)
    {
        printf("Terminating Child, PID=%d\n", getpid());
        exit(0);
    }
    else
    {
        printf("Running Parent, PID=%d\n", getpid());
        while (1)
            ; 
    }
}