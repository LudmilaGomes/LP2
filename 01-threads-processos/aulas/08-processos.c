#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <wait.h>

#define QTD_PROC 5

int main()
{
    int pid_o = getpid();    
    int ret_val;
    int proc_clone[5];

    for (int i = 0; i < QTD_PROC; i++)
    {
        if(pid_o == getpid()) // original
        {   
            ret_val = fork();
            if(ret_val == 0) // estamos no processo clonado
            {
                printf("Processo clonado %d\n", i);
                sleep(1);
            }
            else // estamos no processo original
            {
                proc_clone[i] = ret_val;
            }
        }
    }

    return 0;
}