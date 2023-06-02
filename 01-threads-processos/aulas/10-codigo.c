#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>

// Prova 1: uso de threads e processos e diferenças entre ambos;

/*

Execução concorrente se dará a partir do uso de funções que serão previamente definidas
para serem executadas com concorrência.

Quando a thread1 estiver dentro da seção crítica, a thread2 não pode estar dentro da
seção crítica; elas não podem acessar ao mesmo tempo uma mesma região de memória

Para resolver isso, usamos pthread_mutex_t.

*/

int var_global = 0;
int threads_ativas = 1;
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

void* funcao1(void* p)
{
    long var_local_1 = 0;

    while (threads_ativas)
    {
        var_local_1++;
        printf("[#1]  var_local_1 = %ld\n", var_local_1);
        usleep(50);

        pthread_mutex_lock(&mutex);
        // início - seção crítica
        var_global++;
        printf("[#1]  var_global = %d\n", var_global);
        // fim - seção crítica
        pthread_mutex_unlock(&mutex);

        usleep(50);
    }
    printf("finalizando funcao1.\n");
    return (void*)var_local_1;
}

void* funcao2(void* p)
{
    long var_local_2 = 0;

    while (threads_ativas)
    {
        pthread_mutex_lock(&mutex);
        // início - seção crítica
        var_global++;
        printf("[#2]  var_global = %d\n", var_global);
        // fim - seção crítica
        pthread_mutex_unlock(&mutex);
        
        usleep(50);
        var_local_2++;
        printf("[#2]  var_local_2 = %ld\n", var_local_2);
        usleep(50);
    }
    printf("finalizando funcao2.\n");
    return (void*)var_local_2;
}

int main()
{
    pthread_t thread1, thread2;
    int exec_tempo = 0;
    int res_1, res_2; // capturam o valor de retorno das funções executadas pelas threads

    printf("digite quantos segundos deseja executar: ");
    scanf("%d", &exec_tempo);

    /* pthread_create: passamos a thread, a informação passada para a thread, 
    a função associada à thread e a informação passada para a função (parâmetro) */
    pthread_create(&thread1, NULL, funcao1, NULL); // thread1 executa a funcao1
    pthread_create(&thread2, NULL, funcao2, NULL); // thread2 executa a funcao2

    sleep(exec_tempo);
    printf("[main] tempo de %d segundos esgotado\n", exec_tempo);

    threads_ativas = 0;

    pthread_join(thread1, (void*)&res_1);
    pthread_join(thread2, (void*)&res_2);

    printf("[main] Threads finalizadas: res_1 = %d, res_2 = %d\n", res_1, res_2);
    printf("[main] var_global = %d\n", var_global);
    printf("[main] Vazei...\n");

    return 0;
}

/*

exemplo de saída do programa:

    [main] Threads finalizadas: res_1 = 4719, res_2 = 4719
    [main] var_global = 9421

O valor de var_global deveria, em tese, ser igual à soma de res_1 e res_2, já
que ambas as funções das duas threads incrementam var_global...

Isso acontece pois a instrução "var_global++;" (presente nas duas funções!) não 
é uma instrução atômica, isto é, ela demanda mais de um ciclo de processamento;
pode acontecer que a thread seja escalonada no meio da execução do incremento de
var_global. 

Resumindo: é como se interrompêssemos a execução da instrução de incremento de
var_global; então, a mudança do valor nem chega a ser salvo. Por isso o valor final
de var_global é diferente da soma de res_1 e res_2.

!!!É UM PROBLEMA DE INCONSISTÊNCIA DE MEMÓRIA BASEADO NO USO INCORRETO DE INCREMENTO!!!

Como resolver: uso de incremento atômico;

*/