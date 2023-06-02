// aula do dia 19 de abril

// problema da seção crítica resolvido por meio de uma trava feita pelo próprio programador (spinlock) com uso do __atomic_test_and_set()
#include <stdio.h>
#include <unistd.h>
#include <pthread.h>
#include <stdlib.h>

#define QTD_THREADS 10

/*

if(lock == 0) (I)
    lock = 1; // lock
    // critical section instructions
    lock = 0; // unlock
else
    while(lock == 1);
    // wait
    // go back

(I) - é um grupo de pelo menos três instruções que devem ser
executadas de forma atômica para garantir que o bloqueio seja
efetivo; então, precisamos que elas sejam atômicas; precisamos
de uma instrução atômica para leitura-teste-escrita

*/

/*

<await (!lock) lock = true> (Protocolo de Entrada)
seção crítica
lock = false (Protocolo de Saída)
seção não crítica

uso da função __atomic_test_and_set() para tornar as instruções while e a mudança de lock atômicas        
< while (!lock); lock = 1; >

*/

int var_global = 0;
int lock = 0;

void* func(void* p)
{
    // <await (!lock) lock = true> (Protocolo de Entrada)
    // seção crítica
    // lock = false (Protocolo de Saída)
    // seção não crítica
    
    // uso da função test and set para tornar as instruções while e mudança de lock atômicas        
    // < while (lock); lock = 1; >
    int index = (int) p;
    int counter = 0;
    while (counter < 100000) // cont final * num threads
    {
        //printf("Sou a thread %d (%d)\n", index, counter);
        counter++;
        while (__atomic_test_and_set(&lock, 0)); // PE
        var_global++; // seção crítica
        lock = 0; // PS
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

    printf("[main] var_global = %d\n", var_global);

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

*/

/*

Como nosso código está funcionando:

    for (int i = 0; i < 1000000; i++)
        {
            while (__atomic_test_and_set(&lock, 0));
            number++;
            lock = 0;
        }

lock = 0;
thread 1 - chega primeiro
    lock = 1;
    instrução da seção crítica
    lock = 0;
thread 2 - chega depois
    espera enquanto lock == 1; ao fim, lock == 0
    lock = 1;
    instrução da seção crítica
    lock = 0;

considerando a instrução __atomic_test_and_set()

lock = 0;
thread 1 - chega primeiro
thread 2 - chega depois

thread 1
    while(__atomic_test_and_set(&lock, 0)); 
    1 it - testa se lock == 0; retorna 0; lock = 1; sai do laço pois: (lock == 0); e agora lock == 1;
    entra na seção crítica e lock = 1;
    executa instruções da seção crítica
    sai da seção crítica e lock = 0;
    ...

thread 2
    while(__atomic_test_and_set(&lock, 0)); 
    1 it - testa se lock == 0; retorna 1; lock = 1;
    2 it - testa se lock == 0; retorna 1; lock = 1;
    thread 1 sai da seção crítica e lock == 0;
    3 it - testa se lock == 0; retorna 0; lock = 1; sai do laço pois: (lock == 0); e agora lock == 1;
    entra na seção crítica e lock = 1;
    executa instruções da seção crítica
    sai da seção crítica e lock = 0;
    ...

...

*/