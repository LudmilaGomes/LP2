// aula do dia 19 de abril

// algoritmo de Ticket: cada thread tem um índice e elas executam a partir do uso desses índices
// o algoritmo é utilizado para garantir que todas as threads vão executar; uso da instrução __atomic_fetch_add()
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>

#define QTD_THREADS 2

/*
int number = 1;
int next = 1;
int turn[QTD_THREADS] = 0;

process CS[i 1 to n]
{
    while(true)
    {
        <turn[i] = number++;>
        <await (turn[i] == next)>
        // seção crítica (?)
        <next++;>
    }
}
*/

int number = 0;
int next = 0;
int turn[QTD_THREADS] = {0};

long var_global = 0;

void* func(void* p)
{
    long i = (long) p;
    int counter = 0;
    
    while (counter < 1000000)
    {
        turn[i] = __atomic_fetch_add(&number, 1, 0); // valor buscado, valor de incremento, parâmetro
        while ( turn[i] != next );
        // seção crítica
        var_global++;
        // seção não crítica
        __atomic_fetch_add(&next, 1, 0);
        counter++;
    }   
}

int main()
{
    pthread_t threads[QTD_THREADS];

    for (int i = 0; i < QTD_THREADS; i++)
    {
        pthread_create(&threads[i], NULL, func, (void*) i);
    }
    
    for (int i = 0; i < QTD_THREADS; i++)
    {
        pthread_join(threads[i], NULL);
    }   

    printf("[main] var_global = %ld\n", var_global);

    return 0;
}

/*

Como funciona __atomic_test_and_set()

test-and-set(<mem-loc>)
    return current value in <mem-loc>
    set <mem-loc> to 1

 - a função test-and-set tem como parâmetro um local da memória;
 - retorna o atual valor que está no espaço de memória;
 - atribui ao espaço de memória o valor 1;


Como funciona __atomic_fetch_add()

fetch-and-add(<mem-loc>, <value>)
    return current value in <mem-loc>
    increment <mem-loc> with <value>

 - a função test-and-set tem como parâmetro um local da memória;
 - retorna o atual valor que está no espaço de memória;
 - atribui ao espaço de memória o valor de value;

*/

/*

Análise do código

int number = 0;
int next = 0;
int turn[QTD_THREADS] = {0};

    turn[i] = __atomic_fetch_add(&number, 1, 0);
    while ( turn[i] != next );
    // seção crítica
    __atomic_fetch_add(&next, 1, 0);
    counter++;


thread 1 chega primeiro
thread 2 chega depois

turn[i] recebe o valor de number e depois number é incrementado:

turn[1] == 0, number == 0, next == 0
    //__atomic_fetch_add
turn[1] == 0, number == 1, next == 0
como turn[1] == next, thread 1 entra na seção crítica

turn[2] == 1, number == 2, next == 0
thread 2 espera enquanto turn[2] != next

quando a thread 1 sai da seção crítica: next = 1

*/