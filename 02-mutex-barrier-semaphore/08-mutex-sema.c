#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <semaphore.h>

// semáforo para resolver o problema da seção crítica; uso para exclusão mútua (como lock)

/*

Uso de semáforos:

    #include <pthread.h>
    #include <semaphore.h>

    sem_init()

    sem_t - tipo semáforo

    para usar sem_wait(), o semáforo já deve ter sido inicializado (sem_init())

*/

#define QTD_THREADS 5

sem_t semaforo;

void* th_func(void* arg)
{
    long id = (long) arg;

    printf("[%ld] Aguardando...\n", id);

    sem_wait( &semaforo ); // protocolo de entrada
    printf("[%ld] Dentro da Secao Critica.\n", id);
    sleep(2);
    sem_post( &semaforo ); // protocolo de saída

    pthread_exit(NULL);
}

int main()
{
    pthread_t threads[QTD_THREADS];
    sem_init(&semaforo, 0, 1); // endereço do semáforo, argumento para uso do semáforo, valor de inicialização

    for (long i = 0; i < QTD_THREADS; i++)
    {
        pthread_create(&threads, NULL, th_func, (void*) i);
    }
    
    for (long i = 0; i < QTD_THREADS; i++)
    {
        pthread_join(threads[i], NULL);
    }
    
    return 0;
}