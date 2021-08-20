#include "csapp.h"

int main(void)
{
    int child_status;

    if(fork() == 0)
    {
        printf("HC: hello from child\n");
        exit(0);
    }
    else
    {
        printf("Parent process is waiting !\n");
        wait(&child_status);
        printf("Child process is terminated\n");

        exit(0);
    }

    return 0;
}