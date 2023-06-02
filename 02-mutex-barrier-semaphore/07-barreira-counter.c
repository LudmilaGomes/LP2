#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>

// implementação de barreira com contador compartilhado (shared counter)

#define QTD_THREADS 5

/*
    process work[i=1 to n]
    {
        while(true)
        {
            job();
            <contador++>
            <await contador == n>
        }
    }
*/

int global_counter;

void* job_function(void* p) 
{
	long index = (long)p;

	printf("[%ld] job() - start\n", index);

	//simula job
	sleep(((int)index)+5);

	printf("[%ld] job() - finished\n", index);

	__atomic_fetch_add(&global_counter, 1, 0);

	printf("[%ld] barrier (%d)\n", index, global_counter);

	while (global_counter != QTD_THREADS);
	printf("[%ld] passed barrier\n", index);
}

int main(void) 
{
	pthread_t threads[QTD_THREADS];
	global_counter = 0;

	for (long i = 0; i < QTD_THREADS; i++) 
    {
		pthread_create(&threads[i], NULL, job_function, (void*)i);
	}

	for(int i = 0; i < QTD_THREADS; i++) 
    {
		pthread_join(threads[i], NULL);
	}

	printf("[main] finishing...\n");

	return 0;

}