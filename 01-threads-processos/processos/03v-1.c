// EXPLICAÇÃO DA MEMÓRIA DOS PROCESSOS DO PROGRAMA

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
//#include <wait.h>

int main()
{
    int id = fork(); // creates a child process that have the same memry of parent process
    int n;

    if(id == 0) // child process
    {
        n = 1;
    }
    else // parent process (main)
    {
        n = 6;
    }

    // então, dizemos para a main esperar: 
    if(id != 0)
    {
        wait(); // stop the execution until child process finish itself execution
    }

    for (int i = n; i < n + 5; i++)
    {
        printf("%d ", i);
        fflush(stdout);
    }

    if(id != 0)
    {
        printf("\n");
    }

    return 0;
}