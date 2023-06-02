// EXPLICAÇÃO DA FUNÇÃO 'FORK()' E IDs DOS PROCESSOS PAI E FILHO

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>

int main()
{
    // fork() cria uma 'bifurcação': um processo filho é criado e começa a executar a partir da chamada de fork
    int id = fork(); // em resumo: cria outro processo
    printf("Hello, world from id: %d!\n", id);

    /*
    exemplo de execução:
        Hello, world from id: 9311!     (parent process id)
        Hello, world from id: 0!        (child process id)
    */

    if(id == 0) // child process
    {
        printf("Hello from the child process.\n");
    }
    else
    {
        printf("Hello from the main process.\n\n");
    }

    return 0;
}