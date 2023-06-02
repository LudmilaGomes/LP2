// aula do dia 26 de abril

// barrier
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <time.h>

/*

process Worker[i=1 to n]
{
    while(true)
    {
        job();
        ponto de embarreiramento (só permite que a thread realize novamente a função job() após todas as outras threads terem também executado job())
    }
}

barrier garante entrada eventual de todas as threads na seção crítica

*/

#define QTD_THREADS 10
#define SLEEP_MAX 10
#define MAX_REPEAT 5

pthread_barrier_t my_barrier;

void* job_function(void* arg)
{
    long index = (long) arg;
    int counter = 0;

    //int _sleep = ((int) index) + 1;
    while (counter < MAX_REPEAT)
    {
        int _sleep = rand() % SLEEP_MAX;
        _sleep++;

        printf("[%ld] job(%d)\n", index, _sleep);
    
        // simula job
        sleep(_sleep);

        printf("[%ld] finished job() - barrier\n", index);
        pthread_barrier_wait( &my_barrier );
        // após todas as threads finalizarem, elas são liberadas
        printf("[%ld] left barrier\n", index);
        counter++;
    }
}

int main()
{
    time_t t;
    pthread_t threads[QTD_THREADS];

    pthread_barrier_init(&my_barrier, 0, QTD_THREADS);

    srand(time(&t));

    for (long int i = 0; i < QTD_THREADS; i++)
    {
        pthread_create(&threads[i], NULL, job_function, (void*)i);
    }
    
    for (int i = 0; i < QTD_THREADS; i++)
    {
        pthread_join(threads[i], NULL);
    }

    pthread_barrier_destroy( &my_barrier );

    printf("[main] Finished\n");

    return 0;
}

/*

Sabemos que threads podem compartilhar variáveis.

Mas o que acontece se uma thread lê um resultado antes
que outra thread escreva o valor?

Nessas situações, precisamos sincronizar a execução das
threads.

A forma mais simples de realizar a sincronização das
threads é com o uso de barrier (barreiras)

Barreira é um ponto do programa em que todas as
threads param e esperam; quando todas as threads 
tiverem chegado à barreira, elas podem proceder

======== // ========

Nos casos em que você deve esperar que várias tarefas sejam 
concluídas antes que uma tarefa geral possa prosseguir, a sincronização 
de barreira pode ser usada. Os encadeamentos POSIX especificam 
um objeto de sincronização chamado barreira, juntamente com 
funções de barreira. As funções criam a barreira, especificando 
o número de threads que são sincronizadas na barreira e 
configuram os threads para executar tarefas e aguardar na 
barreira até que todos os threads alcancem a barreira. Quando 
a última thread chega à barreira, todas as threads retomam a execução.

*/