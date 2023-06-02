#include <stdio.h>
#include <pthread.h>

#define QTD_THREADS 5

void* func_t()
{
    printf("Oi\n");
}

int main()
{
    pthread_t t[QTD_THREADS];

    for (int i = 0; i < QTD_THREADS; i++) // criamos 'QTD_THREADS' threads
    {
        if (pthread_create(&t[i], NULL, func_t, NULL) == -1) // verifica se a criação das threads não teve erro
        {
            printf("Erro ao criar thread %d.\n", i);
            return 1;
        }
    }
    
    for (int i = 0; i < QTD_THREADS; i++)
    {
        pthread_join(t[i], NULL);
    }
    
    return 0;
}
/*

Este programa possui 6 threads funcionando:

5 (valor definido na constante que indica o número de threads)
threads que irão executar uma função definida no programa;

a última é a thread principal, que tem a função de criar e gerenciar
as threads criadas;

*/

/*

sobre pthread_join(), apenas relembrando:

t1, t2, t3, sendo a t1 a principal; t1 contêm a instrução exit(0);
quando t1 chegar ao fim, todas as threads serão encerradas;
mas e se t1 chega ao fim e t2 e t3 ainda não tiveram retorno, isto é,
ainda não terminaram de executar? Assim, surge um problema.
Esse problema de t1 terminar e interromper a execução de t2 e t3
é resolvido com o uso de 'pthread_join()', que faz com que a thread
principal espere pelo fim da execução da thread que foi passada no 
argumento de 'pthread_join()'.

*/