#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>

#define QTD_THREADS 5

void* funcao_thread(void* args) {
	long index = (long) args;
	int counter = 0;

	while(counter < 10000) {
		printf("[t] Sou a thread %ld (%d)\n", index, counter);
		counter++;
		usleep(50);
	}
	return (void*)100+index;
}

int main() {
	pthread_t threads[QTD_THREADS];
	int thread_ret_val[QTD_THREADS];

	printf("[m] programa iniciado\n");

	for (long i = 0; i < QTD_THREADS; i++) {
		pthread_create(&threads[i], NULL, funcao_thread, (void*)i);
	}

	for (int i = 0; i < QTD_THREADS; i++) {
		pthread_join(threads[i], (void**) &thread_ret_val[i]);
		printf("[m] thread %d retornou %d\n", i, thread_ret_val[i]);
	}

	return 0;
}
