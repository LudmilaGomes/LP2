// EXPLICAÇÃO DO COMANDO execl()

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

int main(int argc, char* argv[])
{
    int id = fork();
    
    if(id == -1)
    {
        return 1;
    }

    if(id == 0) // child process
    {
        execlp("ping", "ping", "-c", "3", "google.com", NULL);
    }
    else // you can't use execl() on the parent process, cause this will be replaced, so the program won't work properly
    {
        wait(NULL);
        printf("Success!\n");
    }

    /*
    execlp("ping", "ping", "-c", "3", "google.com", NULL);
    printf("Success!\n"); // doesn't appear on display

    execl() replace all whole process, all the memory will be replace
    after the command execl(); so, we need to create a new process (use
    fork() to do it!).
    */

    return 0;
}