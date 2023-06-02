#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <signal.h>
#include <sys/wait.h>

#define QTD_PROCESSES 7

int main()
{
    int main_id = getpid();
    int pids[QTD_PROCESSES];

    // criamos os processos
    for(int i = 0; i < QTD_PROCESSES; i++)
    {
        if(getpid() == main_id)
        {
            pids[i] = fork();
        }
    }

    if(getpid() != main_id) // apenas os processos filhos executam as instruções do if
    {
        printf("Oi\n"); // são exibidos QTD_PROCESSES "Oi"
    }

    // esperamos até o último processo
    waitpid(pids[QTD_PROCESSES-1], NULL, 0);

    //matamos os processos
    if(getpid() == main_id)
    {
        for (int i = 0; i < QTD_PROCESSES; i++)
        {
            kill(pids[i], SIGHUP);
        }
        printf("Oi\n");
        printf("[main] saindo...\n");
        exit(0);
    }

    return 0;
}

/*

Temos o processo original funcionando; é este processo que irá
criar (ele faz a chamada do sistema operacional para criação de
novos processos) os outros processos; vamos assumir que ele tem
a função de somente criar e finalizar os processos filhos, e fazemos
com que o nosso código tenha instruções específicas apenas para
os processos filhos realizarem, deixando o processo original com
a função de gerenciar novos processos criados.

*/