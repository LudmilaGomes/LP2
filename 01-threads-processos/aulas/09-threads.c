#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>

// void* - ponteiro para nenhum tipo (aponta para o endereço de um tipo qualquer)

void* funcao_thread(void* p) // usamos o parâmetro p como identificador das threads
{
    long index = (long)p;

    printf("Sou a thread[%ld]\n", (long)p);
    sleep(2);
    printf("Adios...[%ld]\n", (long)p);

    exit(0);
    // executamos exit() quando queremos matar todo o processo;
    // return só finaliza uma thread.
}

int main()
{
    pthread_t threads[5];

    for (long i = 0; i < 5; i++)
    {
        pthread_create(&threads[i], NULL, funcao_thread, (void*)i);
    }   

    for (long i = 0; i < 5; i++)
    {
        pthread_join(threads[i], NULL);
    }

    /*
    
    Usamos dois laços de repetição
    Se usassemos apenas um for e chamassemos pthread_create e pthread_join 
    nesse for, a thread1 seria iniciada e se esperaria que ela acabasse, e
    assim por diante, ou seja, não existiria concorrência, cada thread seria
    executada uma por vez.

    */

    printf("[main] Saindo...\n");

    return 0;
}