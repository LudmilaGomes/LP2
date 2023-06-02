#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <semaphore.h>

/*
    Para uso correto do semáforo como barreira que consiga funcionar
    adequadamente para mais de duas threads, usaremos uma thread
    coordenadora e um contador (que contará a quantidade de threads
    que chegaram na barreira)
*/

#define QTD_THREADS 5
sem_t mutex, barrier;
int contador = 0;

void* func(void* arg)
{
    long indice = (long) arg;

    while(1)
    {    
        sleep(2);
        printf("[thread %ld] Cheguei na barreira...\n", indice);
        
        sem_wait(&mutex);
        contador++; // incrementamos a cada vez que uma thread chega na barreira
        sem_post(&mutex);

        // quando a última thread chega na barreira, ela libera as outras com sem_post(&barrier)
        if (contador == QTD_THREADS) 
        {
            sem_post(&barrier);
        }

        sem_wait(&barrier); // threads chegam na barreira
        
        printf("[thread %ld] Sai da barreira...\n", indice);

        sem_wait(&mutex);
        contador--; // decrementamos a cada vez que uma thread sai da barreira
        sem_post(&mutex);

        // quando todas as threads tiverem saído da barreira:
        if (contador == 0) 
        {
            sem_init(&barrier, 0, 0); // redefinimos o semáforo barrier
        }
    }
}

int main()
{
    pthread_t threads[QTD_THREADS];
    sem_init(&mutex, 0, 1);
    sem_init(&barrier, 0, 0);

    for (long i = 0; i < QTD_THREADS; i++)
    {
        pthread_create(&threads[i], NULL, func, (void*) i);
    }

    for (long i = 0; i < QTD_THREADS; i++)
    {
        pthread_join(threads[i], NULL);
    }

    return 0;
}

/*

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <semaphore.h>

// semáforo como barreira do ChatGPT

#define QTD_THREADS 5

sem_t mutex, barrier;
int contador = 0;

void* func(void* arg) {
    long indice = (long) arg;

    while(1)
    {
        sleep(2);
        printf("[thread %ld] Cheguei na barreira...\n", indice);
        
        sem_wait(&mutex);
        contador++;
        sem_post(&mutex);

        if (contador == QTD_THREADS) {
            sem_post(&barrier);
        }

        sem_wait(&barrier);
        sem_post(&barrier);
        
        printf("[thread %ld] Sai da barreira...\n", indice);

        sem_wait(&mutex);
        contador--;
        sem_post(&mutex);

        if (contador == 0) {
            sem_init(&barrier, 0, 0); // Redefine o semáforo barrier
        }
    }
}

int main() {
    pthread_t threads[QTD_THREADS];
    sem_init(&mutex, 0, 1);
    sem_init(&barrier, 0, 0);

    while (1) {
        for (long i = 0; i < QTD_THREADS; i++) {
            pthread_create(&threads[i], NULL, func, (void*) i);
        }

        for (long i = 0; i < QTD_THREADS; i++) {
            pthread_join(threads[i], NULL);
        }

        if (contador == 0) {
            break; // Sai do loop quando todas as threads terminarem
        }
    }

    return 0;
}


*/