#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <time.h>
#include <pthread.h>
#include <semaphore.h>

/*
(Questão 2 - 5,0)
Escreva um programa em C/C++ que simule o funcionamento de um sistema de 
pedágio em uma rodovia. O programa deve usar threads para representar os 
veículos que passam pelo pedágio e deve explorar a solução do problema da 
seção crítica (solucionando com trava ou semáforo) e sincronismo condicional 
(usando barreira).
 
Cada veículo deve escolher aleatoriamente uma das cabines de pedágio para 
passar e deve esperar sua vez para pagar o pedágio. Quando um veículo paga 
o pedágio, ele deve incrementar o valor total arrecadado pelo pedágio. 
O programa deve garantir que apenas um veículo por vez possa pagar o pedágio 
em cada cabine (usando trava ou semáforo para solucionar o problema da 
seção crítica).
 
Além disso, o programa deve usar uma barreira para sincronizar os veículos 
de forma que eles só possam sair do pedágio depois que todos os veículos 
tenham pago o pedágio. Depois que todos os veículos saírem do pedágio, o 
programa deve imprimir o valor total arrecadado pelo pedágio.
 
O programa deve terminar quando todos os veículos tiverem passado pelo pedágio.
*/

#define QTD_THREADS 10
#define VALOR_PEDAGIO 1000000

pthread_barrier_t barrier;
int cabine_ped[5];
// cada cabine tem sua própria trava
int lock_cabine[5] = {0}; // representa uma trava para cada cabine
int arrecadacao = 0;

void* func(void* arg)
{
    long indice = (long) arg;

    int cab_aleat = rand() % 5; // cabine de 0 a 4
    printf("Thread [%ld] escolheu cabine %d\n", indice, cab_aleat);

    // se a cabine estiver ocupada, a thread espera até que seja desocupada
    while(__atomic_test_and_set(&lock_cabine[cab_aleat], 0));   // verifica se a cabine escolhida está ocupada
    arrecadacao += VALOR_PEDAGIO;                               // seção crítica
    lock_cabine[cab_aleat] = 0;                                 // cabine escolhida está disponível

    printf("Thread [%ld] esperando para sair...\n", indice);    // chega na barreira
    pthread_barrier_wait(&barrier);                             // threads ficam embarreiradas
    printf("Thread [%ld] saindo do pedagio...\n", indice);      // sai da barreira
}

int main()
{
    pthread_t threads[QTD_THREADS];
    time_t t;

    pthread_barrier_init(&barrier, 0, QTD_THREADS);
    srand(time(&t));

    for (long i = 0; i < QTD_THREADS; i++)
    {
        pthread_create(&threads[i], NULL, func, (void*) i);
    }

    for (int i = 0; i < QTD_THREADS; i++)
    {
        pthread_join(threads[i], NULL);
    }

    printf("Valor total arrecadado = %d\n", arrecadacao);

    return 0;
}

/*

Uma das saídas obtidas:

Thread [3] escolheu cabine 1
Thread [7] escolheu cabine 1
Thread [6] escolheu cabine 1
Thread [6] esperando para sair...
Thread [4] escolheu cabine 1
Thread [4] esperando para sair...
Thread [9] escolheu cabine 4
Thread [9] esperando para sair...
Thread [2] escolheu cabine 1
Thread [2] esperando para sair...
Thread [3] esperando para sair...
Thread [7] esperando para sair...
Thread [0] escolheu cabine 4
Thread [0] esperando para sair...
Thread [8] escolheu cabine 4
Thread [8] esperando para sair...
Thread [1] escolheu cabine 4
Thread [1] esperando para sair...
Thread [5] escolheu cabine 2
Thread [5] esperando para sair...
Thread [4] saindo do pedagio...

(todas saem do pedágio)

*/