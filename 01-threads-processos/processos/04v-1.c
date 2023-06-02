// EXPLICAÇÃO DOS IDs DOS PROCESSOS, USO DO 'GETPID()' E 'GETPPID()'

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

int main()
{
    int id = fork();

    if(id == 0) // processo filho espera 1 segundo e é executado depois do processo pai
    {
        sleep(1);
        /*
        quando fazemos isso, o processo filho espera 1 segundo e é executado depois 
        do processo pai, mas o processo pai já foi executado e terminado. Assim, nesses
        casos, o processo filho exibe o ID de outro processo pai que não o processo original.
        */
    }

    printf("Current ID: %d, parent ID: %d\n", getpid(), getppid());

    /*
        Com isso, o processo pai executa e exibe sua mensagem do printf primeiro (antes
        do processo filho), mas espera pelo fim da execução do processo filho para só
        depois ser terminado.

        A função 'wait()' faz o processo que a chama esperar pelos processos filhos terem
        terminado para só então terminar o processo original (que chamou a função).

        Agora, o ID do processo pai do processo filho ('getppid()') é exibido corretamente.
    */
    /*
    if(id != 0)
    {
        wait(NULL);
    }
    */

    int result = wait(NULL);

    if(result == -1)
    {
        printf("No children to wait for.\n");
    }
    else
    {
        printf("%d finished execution.\n", result);
    }
     
    return 0;
}