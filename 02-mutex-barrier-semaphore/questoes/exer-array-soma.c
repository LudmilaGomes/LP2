// aula do dia 3 de maio

/*

Exercício 1:

algoritmo para calcular a soma dos elementos de um array
4 threads 
array de 8 elementos
cada thread trabalha com uma parte do array
cada thread realiza a soma de 2 elementos do array
somatório geral deve ser apresentado

usar mutex e barrier

*/

// acessamos um elemento do array pelo (índice da thread + [i_contador * quantidade de threads]):
// 0 1 2 3 4 5 6 7
// 0       4
//   1       5
//     2       6
//       3       7

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>

#define TAM_ARRAY 8
#define QTD_THREADS 4
#define QTD_ELEM_ACESS 2

//int TAM_ARRAY; // tamanho máximo do array
//int QTD_THREADS = TAM_ARRAY/2; // quantidade de threads utilizadas
//int QTD_ELEM_ACESS = QTD_THREADS/2; // quantidade de elementos acessados por cada thread

int ar[TAM_ARRAY];
int soma = 0;
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

void* func(void* arg)
{
    int cont_vez = 0;

    int acesso_thread = (int) arg;

    for (int i = 0; i < QTD_ELEM_ACESS; i++)
    {
        pthread_mutex_lock( &mutex );
        soma += ar[acesso_thread + (QTD_THREADS * i)];
        pthread_mutex_unlock( &mutex );
    }
}

int main()
{
    pthread_t threads[QTD_THREADS];

    for (int i = 0; i < TAM_ARRAY; i++)
    {
        ar[i] = i;
    }

    printf("Elementos do array: ");
    for (int i = 0; i < TAM_ARRAY; i++)
    {
        printf("%d ", ar[i]);
    }
    printf("\n");

    for (int i = 0; i < QTD_THREADS; i++)
    {
        pthread_create(&threads[i], NULL, func, (void*)i);
    }
    
    for (int i = 0; i < QTD_THREADS; i++)
    {
        pthread_join(threads[i], NULL);
    }

    printf("A soma dos elementoss do array foi: %d\n", soma);

    return 0;
}