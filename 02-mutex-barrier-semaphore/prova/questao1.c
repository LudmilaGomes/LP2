#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <time.h>
#include <semaphore.h>

/*
(Questão 1 - 5,0)
Escreva um programa em C que simule um sistema de detecção de 
vazamento em uma represa. O programa deve usar threads POSIX e 
espera ocupada para detectar mudanças em uma variável que 
representa o nível de água da represa (valor inicial: 1000 
litros). O nível de água é modificado por um conjunto de 5 
threads. Cada thread receberá um valor aleatório do quanto 
incrementará no nível do reservatório (entre 10 e 100 litros) 
e dormirá um tempo aleatório (entre 0 e 300ms) entre cada 
incremento. Quando o nível de água atingir 5000 litros, essas 
threads devem ser bloqueadas até que a thread principal garanta 
que o nível do reservatório foi reestabelecido para 1000 litros 
(e depois dormir 200ms). Deve ser utilizada espera ocupada em todos 
os casos e funções atômicas para modificação do nível do reservatório.
*/

#define QTD_THREADS 5
#define INC_ALEATOR 91
#define TEM_ALEATOR 301

int nivel_agua = 1000;
int lock = 0;
sem_t semaf;
pthread_barrier_t barrier;

void* func(void* arg)
{
    // o programa funciona em loop infinito
    while(1)
    {
        int valor = (rand() % INC_ALEATOR) + 10; 

        // uso de espera ocupada para garantir que apenas uma thread por vez acesse a seção crítica
        while(__atomic_test_and_set(&lock, 0)); // protocolo de entrada
        nivel_agua += valor;                    // secao critica
        lock = 0;                               // protocolo de saída

        printf("[thread] Nivel da agua: %d\n", nivel_agua);
        int dorme = rand() % TEM_ALEATOR;
        usleep(dorme);

        // as threads verificam se nivel_agua >= 5000; se sim, ficam embarreiradas
        if (nivel_agua >= 5000)
            sem_wait(&semaf);
    }
}

int main()
{
    pthread_t threads[QTD_THREADS];
    pthread_barrier_init(&barrier, 0, QTD_THREADS);

    time_t t;

    srand(time(&t));

    for (int i = 0; i < QTD_THREADS; i++)
    {
        pthread_create(&threads[i], NULL, func, NULL);
    }
    
    while(1)
    {
        // uso de espera ocupada enquanto (nivel_agua < 5000)
        while(nivel_agua < 5000);
        printf("[MAIN] Nivel da agua: %d - Baixando nivel...\n", nivel_agua);
        sem_post(&semaf); // as threads estavam embarreiradas; saem da barreira
        nivel_agua = 1000; // nivel_agua retorna a 1000
        usleep(200);
    }

    return 0;
}