// SOBRE CONDIÇÕES DE CORRIDA e MUTEX (vídeos 03 e 04)
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

int var = 0;
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

void* func()
{
    for (int i = 0; i < 100000; i++)
    {
        pthread_mutex_lock( &mutex );
        var++;
        pthread_mutex_unlock( &mutex );
    }
}

int main()
{
    pthread_t t1, t2;

    if(pthread_create(&t1, NULL, &func, NULL) != 0)
    {
        return 1;
    }

    if(pthread_create(&t2, NULL, &func, NULL) != 0)
    {
        return 2;
    }

    if(pthread_join(t1, NULL) != 0)
    {
        return 3;
    }

    if(pthread_join(t2, NULL) != 0)
    {
        return 4;
    }

    printf("Number of (var) = %d\n", var);
    // Number of (var) = 148247

    return 0;
}
/*

Como as duas threads estão executando ao mesmo tempo e acessando, então,
a memória ao mesmo tempo (ambas acessam a variável 'var'). As duas threads
tentam escrever no mesmo espaço de memória de forma simultânea e isso faz
com que alguns incrementos sejam perdidos. Quando uma thread incrementou o 
valor em 'var', ao mesmo tempo, a outra thread já havia incrementado e passado
o novo valor para a memória

t1
var = 25
var++
var = 26
*vamos salvar o valor na memória*

t2 
var = 25
var++
var = 26
*vamos salvar o valor na memória*

E quando a t2 vai salvar o valor na memória, sua execução é interrompida
pela execução da t1, e o incremento realizado acaba sendo perdido

Quando usamos o mutex, impedimos que as threads entrem em uma mesma seção
crítica (que, no caso, seria o incremento de 'var') e atrasamos a execução
de uma thread ou outra em virtude disso, mas garantimos que uma thread não
é interrompida enquanto incrementa e salva o novo valor de 'var'. Assim, os
valores de incremento não serão perdidos e a saída desse programa sempre será
este, a partir do uso do mutex, claro:

    Number of (var) = 200000

*/