#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <semaphore.h>

// semáforo para uso para sincronização condicional (como barrier)

/*

Uso de semáforos:

    #include <pthread.h>
    #include <semaphore.h>

    sem_init()

    sem_t - tipo semáforo

    para usar sem_wait(), o semáforo já deve ter sido inicializado (sem_init())

*/

// só funciona para 2 threads; iremos atualizar para funcionar para n threads
#define QTD_THREADS 2

sem_t semaforo[QTD_THREADS];

void* th_func(void* arg)
{
    long id = (long) arg;

    printf("[%ld] job...\n", id);
    sleep(2);

    // sinaliza a chegada no ponto de embarreiramento
    sem_post( &semaforo[id] ); // realiza o incremento do semáforo

    for (int i = 0; i < QTD_THREADS; i++)
    {
        if(id != i)
            sem_wait( &semaforo[i] ); // realiza o decremento do semáforo
    }
    printf("[%ld] saindo...\n", id);

    pthread_exit(NULL);
}

int main()
{
    pthread_t threads[QTD_THREADS];

    for (int i = 0; i < QTD_THREADS; i++)
    {
        sem_init(&semaforo[i], 0, 0); // inicializamos os semáforos com 0
    }

    for (long i = 0; i < QTD_THREADS; i++)
    {
        pthread_create(&threads[i], NULL, th_func, (void*) i);
    }
    
    for (long i = 0; i < QTD_THREADS; i++)
    {
        pthread_join(threads[i], NULL);
    }
    
    return 0;
}