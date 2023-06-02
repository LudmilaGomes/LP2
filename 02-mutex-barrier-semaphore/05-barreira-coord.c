// aula do dia 26 de abril

// uso da thread coordenadora para coordenar as outras (com barreiras)
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <time.h>

#define QTD_THREADS 5

int arrive[QTD_THREADS] = {0};
int proceed[QTD_THREADS] = {0};

void* job_function(void* arg)
{
    long index = (long) arg;
    
    printf("[%ld] job() - start\n", index);

    // simula job
    sleep(((int)index) + 5);

    printf("[%ld] job() - finished\n", index);
    arrive[index] = 1;

    printf("[%ld] arrive barrier\n", index);    
	while (proceed[index] != 1);
	printf("[%ld] passed barrier\n", index);
	proceed[index] = 0;
}

void* coord_function(void* arg)
{
    for(int i = 0; i < QTD_THREADS; i++) 
    {
		while(arrive[i] != 1);
		arrive[i] = 0;
	}
	printf("[coord] all threads arrived!\n");

	for (int i = 0; i < QTD_THREADS; i++) 
    {
		proceed[i] = 1;
	}
	printf("[coord] threads will proceed\n");
}

int main()
{   
    pthread_t threads[QTD_THREADS];
    pthread_t t_coord;

    for (long i = 0; i < QTD_THREADS; i++)
    {
        pthread_create(&threads[i], NULL, job_function, (void*) i);
    }

    pthread_create(&t_coord, NULL, coord_function, NULL);
    
    for (long i = 0; i < QTD_THREADS; i++)
    {
        pthread_join(threads[i], NULL);
    }
    
    printf("[main] finishing...\n");

    return 0;
}

/*

Usamos uma thread coordenadora para realizar o gerenciamento
da sincronização das demais threads atuantes com uso de barreira.

// são inicializadas com 0
int arrive[QTD_THREADS] = {0};  // indica que a thread[index] chegou na barreira
int proceed[QTD_THREADS] = {0}; // indica que a thread[index] pode prosseguir

void* job_function(void* arg)
{
    long index = (long) arg;

    // job - thread[index] executa instrução
    arrive[index] = 1;           // thread[index] alcança a barreira e arrive[index] = 1
	while (proceed[index] != 1); // loop enquanto proceed[index] == 0, ou seja, não pode prosseguir
    // a thread coordenadora faz proceed[index] = 1 e as threads esperam por isso
	proceed[index] = 0;          // proceed[index] novamente tem o valor 0 para a próxima iteração
}

void* coord_function(void* arg)
{
    // trabalho da thread coordenadora:

    for(int i = 0; i < QTD_THREADS; i++) 
    {
		while(arrive[i] != 1);  // aguarda todas as threads chegarem na barreira
		arrive[i] = 0;          // quando todas as threads chegarem na barreira, executa
        // arrive[i] = 0 é feito para a próxima iteração
	}

	for (int i = 0; i < QTD_THREADS; i++) 
    {
        // após todas as threads terem alcançado a barreira, indicamos:
		proceed[i] = 1;
        // as threads podem prosseguir
	}
}

*/