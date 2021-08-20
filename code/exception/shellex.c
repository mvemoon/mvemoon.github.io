#include "csapp.h"
#define MAXARGS 128

void eval(char *cmdline);
int parseline(char* buf, char **argv);
int builtin_command(char **argv);

int main(void)
{
    char cmdline[MAXLINE];

    while(1)
    {
        printf(">");
        Fgets(cmdline, MAXLINE, stdin);
        if (feof(stdin))
            exit(0);
        eval(cmdline);
    }
}



void eval(char *cmdline)
{
    char *argv[MAXLINE];
    char buf[MAXLINE];
    int bg;
    pid_t pid;

    strcpy(buf, cmdline);
    bg = parseline(buf, argv);
    
    if(argv[0] == NULL)
    {
        return;
    }

    if(!builtin_command(argv))
    {
        if((pid = Fork()) == 0)
        {
            if (execve(argv[0], argv, environ)<0)
            {
                printf("%s: Command not found.\n", argv[0]);
                exit(0);
            }
        }
    }


    /*Parent wait*/
    if(!bg)
    {
        int status;
        if (waitpid(pid, &status, 0)<0)
        {
            unix_error("waitfg: waitpid error");
        }
        else
        {
            printf("%d %s", pid, cmdline);
        }

        return;

    }
}


/* if the first arg is a built-in command, run it and return true*/
int builtin_command(char **argv)
{
    if(!strcmp(argv[0], "quit"))
        exit(0);
    if(!strcmp(argv[0], "&"))
        return 1;
    return 0;
}

/*parseline - Parse the command line and build the argv array*/
int parseline(char *buf, char **argv)
{
    char *delim;
    char argc;
    char bg;

    buf[strlen(buf)-1] = ' '; /*Replace trailing '\n with space*/
    while (*buf && (*buf == ' '))
    {
        buf++;
    }

    /* Build the argv list */
    argc = 0;
    while((delim = strchr(buf, ' ')))
    {
        argv[argc++] = buf;
        *delim = '\0';//分割字符串
        buf = delim + 1;
        while(*buf && (*buf == ' '))
            buf++;
    }
    argv[argc] = NULL;

    if(argc == 0) /* Ignore blank line*/
        return 1;
    
    /* Should the job run int background*/
    if((bg = (*argv[argc-1] == '&'))!=0)
        argv[--argc] = NULL;

    return bg;
}