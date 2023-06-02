// EXPLICAÇÃO DO COMANDO waitpid()

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <time.h>
#include <sys/wait.h>

int main()
{
    int pid1 = fork();
    if(pid1 == -1)
    {
        printf("Error creating process 1 (child).\n");
        return 1;
    }
    if(pid1 == 0) // 1 (child)
    {
        sleep(4);
        printf("Finished execution (%d)\n", getpid());
        return 0;
    }

    int pid2 = fork();
    if(pid2 == -1)
    {
        printf("Error creating process 2 (child).\n");
        return 2;
    }
    if(pid2 == 0) // 2 (child)
    {
        sleep(1);
        printf("Finished execution (%d)\n", getpid());
        return 0;
    }

    /*
    // wait() retorna o id do processo pelo qual ele espera terminar

    int pid1_res = wait(NULL);
    printf("Waited for %d\n", pid1_res);

    int pid2_res = wait(NULL);
    printf("Waited for %d\n", pid2_res);

    exemplo de saída:
        Finished execution (8585)
        Waited for 8585
        Finished execution (8584)
        Waited for 8584
    */

    wait(NULL);

    int pid1_res = waitpid(pid1, NULL, 0);
    printf("Waited for %d\n", pid1_res);

    int pid2_res = waitpid(pid2, NULL, 0);
    printf("Waited for %d\n", pid2_res);

    /*
    waitpid(): como o wait(NULL), retorna o pid do processo, mas precisamos 
    passar para a função waitpid() outros parâmetros:

    primeiro argumento: indicamos qual o processo a se esperar; podemos usar
    também outros valores para alterar o comportamento normal do waitpid(), que
    é esperar o fim da execução do processo filho indicado, dessa forma:
    <-1 : waitpid() espera qualquer processo filho cujo ID do grupo de processos seja igual ao valor absoluto de pid
    -1  : waitpid() espera por qualquer processo filho
    0   : waitpid() espera qualquer processo filho cujo ID do grupo de processos seja igual ao do processo de chamada
    >0  : waitpid() espera pelo processo filho que possui o valor de pid passado para a função

    segundo argumento: mesmo argumento do wait() (em que usamos NULL também);

    terceiro argumento: indicamos uma opção para waitpid(); quando não usamos
    nenhuma opção, passamos 0; também podemos passar outros parâmetros que dão
    mais utilidades e possibilidades com o uso do waitpid(), como WNOHANG;
    WNOHANG retorna imediatamente se nenhum processo filho terminou

    exemplo de saída:
        Finished execution (8819)
        Finished execution (8818)
        Waited for 8818
        Waited for 8819
    */

    return 0;
}