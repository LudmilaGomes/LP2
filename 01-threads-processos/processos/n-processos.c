// CÓDIGO COM CRIAÇÃO DE n PROCESSOS
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

// queremos um TOTAL de 7 processos, por exemplo

#define QTD_PROCESSES 7

int main()
{
    int main_id = getpid();
    int id = fork();

    if(id == -1)
    {
        printf("Error fork.\n");
        return 1;
    }

    /*
    Como já temos dois processos ( main e child ), criamos (QTD_PROCESSES - 2) 
    processos e temos a quantidade total final desejada de forma correta
    */
    for(int i = 0; i < QTD_PROCESSES - 2; i++)
    {
        if(getpid() == main_id)
        {
            fork();
        }
    }
    printf("Oi\n"); // são exibidos QTD_PROCESSES "Oi"

    return 0;
}