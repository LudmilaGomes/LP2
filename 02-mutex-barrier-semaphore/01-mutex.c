// aula do dia 19 de abril

// problema da seção crítica e como resolver com mutex_lock e unlock
#include <stdio.h>
#include <unistd.h>
#include <pthread.h>
#include <stdlib.h>

#define QTD_THREADS 10

int var_global = 0;

/*
 seção crítica: conjunto de instruções que acessam a memória compartilhada;
 problema: mais de uma thread ao mesmo tempo acessa e escreve numa variável
compartilhada; é um problema, pois, no nosso código exemplo, incrementos da
var_global serão perdidos quando as threads acessarem ela simultaneamente;
 solução: para garantir que apenas uma thread acessará a seção crítica por 
vez, usamos mutex (mutual exclusion), que garante, então, a exclusão mútua 
das threads.
*/

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER; // inicializamos o objeto do tipo mutex

void* func(void* arg)
{
    int index = (int) arg;
    int counter = 0;
    while (counter < 10000) // [cont final * num threads]
    {
        printf("Sou a thread %d (%d)\n", index, counter);
        counter++;
        pthread_mutex_lock( &mutex );   // trava
        var_global += 1; // o uso do mutex impede mais de uma thread de acessar simultaneamente essa instrução
        pthread_mutex_unlock( &mutex ); // destrava
    }
}

int main()
{
    pthread_t threads[QTD_THREADS];

    // criamos as threads
    for (int i = 0; i < QTD_THREADS; i++)
    {
        pthread_create(&threads[i], NULL, func, (void*) i);
    }
    
    // fazemos a thread main esperar o fim da execução das outras threads
    for (int i = 0; i < QTD_THREADS; i++)
    {
        pthread_join(threads[i], NULL);
    }

    // exibimos o valor de var_global
    printf("[main] valor final var_global = %d\n", var_global);

    return 0;
}


/*

Resultado sem uso do mutex: "Number = 70347"

Resultado com uso do mutex: "Number = 100000"

mutex:

mutex é usado para garantir que apenas uma thread estará
realizando instruções de uma seção crítica; essa garantia
é necessária quando estamos realizando escrita em uma variável
compartilhada; se essa variável é acessada para escrita de forma
simultânea por mais de uma thread, algumas alterações vão se
perder (devido à forma que a execução de threads acontece: uma 
execução interrompe outra), e esse funcionamento não é o esperado.
Por isso, então, precisamos garantir que apenas uma thread estará
executando as instruções da seção crítica, e podemos ter essa
garantia com o uso de mutex!

funciona usando a ideia de espera ocupada;

garante exlusão mútua, ausência de impasse, ausência de
atraso desnecessário; mas não garante a entrada eventual
das threads na seção crítica em questão (isso é um problema
e buscamos resolver isso com uso de outras ferramentas)

======== // ========

Use bloqueios de exclusão mútua (mutexes) para serializar a execução do 
encadeamento. Bloqueios de exclusão mútua sincronizam threads, geralmente 
garantindo que apenas um thread por vez execute uma seção crítica do código. 
Os bloqueios mutex também podem preservar o código de thread único.

*/