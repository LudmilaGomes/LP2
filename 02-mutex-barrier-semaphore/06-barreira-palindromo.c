// aula do dia 26 de abril

// 4 threads geram caracteres aleatórios e uma quinta thread testa 
// se a palavra formada pelos caracteres é palíndromo - necessário atualizar
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>

/*
barreira - forma geral

process work[i=1 to n]
{
    while(true)
    {
        job();
        ponto de embarreiramento;
    }
}
*/

#define QTD_THREADS 4
#define MAX_REPEAT 5
#define A_ASCII 65

// usamos 4 threads + 1 thread (verifica se palavra sé palíndromo)
char palavra[QTD_THREADS + 1]; // 4 letras + 1 (\0)

pthread_barrier_t barrier_palavra;
pthread_barrier_t barrier_teste;

void* job_function(void* arg)
{
    long index = (long) arg;
	int counter = 0;

	while (counter < MAX_REPEAT)
    {
		char my_char = ((char)A_ASCII + (rand() % 26));
		
        printf("[%ld] %c\n", index, my_char);

		palavra[index] = my_char;
		
        sleep((int)(index+1));
		
        pthread_barrier_wait(&barrier_palavra);
		printf("[%ld] passed barrier (p)\n", index);
		pthread_barrier_wait(&barrier_teste); // 4 threads aguardam término da verificação da palavra para gerar nova palavra
		printf("[%ld] passed barrier (t)\n", index);
		
        counter++;
    }
}

void* teste_palindromo()
{
    for (int i = 0, j = QTD_THREADS-1; i < QTD_THREADS, j >= 0; i++, j--) 
    {
	    if (palavra[i] != palavra[j]) 
        {
			return 0;
		}
	}
	return 1;
}

int main()
{
    time_t t;
    pthread_t threads[QTD_THREADS];

    // inicializamos as barreiras
    pthread_barrier_init(&barrier_palavra, NULL, QTD_THREADS + 1);
    pthread_barrier_init(&barrier_teste, NULL, QTD_THREADS + 1);

    srand(time(&t));

    // criamos as threads
    for (long i = 0; i < QTD_THREADS; i++)
    {
        pthread_create(&threads[i], NULL, job_function, (void*) i);
    }

    int counter = 0; // contador gerar 5 palavras aleatórias e verificar se é palíndromo
    while (counter < MAX_REPEAT)
    {
        printf("[main] aguardando palavra...\n");

		pthread_barrier_wait(&barrier_palavra); // barreira; main aguarda palavra ser feita
		palavra[QTD_THREADS] = '\0'; // último caractere é caractere finalizador de string
		printf("[main] palavra: %s\n", palavra); // exibimos a palavra

		if (teste_palindromo() == 1) 
        {
			printf("[main] %s = PALINDROMO\n", palavra);
		} 
        else 
        {
			printf("[main] %s = NAO PALINDROMO\n", palavra);
		}

		pthread_barrier_wait(&barrier_teste);
		counter++;
    }
    
    for(int i = 0; i < QTD_THREADS; i++) 
    {
		pthread_join(threads[i], NULL);
	}

	pthread_barrier_destroy(&barrier_palavra);
	pthread_barrier_destroy(&barrier_teste);

	printf("[main] finished\n");

    return 0;
}

/*

#define QTD_THREADS 4
#define MAX_REPEAT 5
#define A_ASCII 65

char palavra[QTD_THREADS + 1]; // 4 letras + 1 (\0)

pthread_barrier_t barrier_palavra;
pthread_barrier_t barrier_teste;

void* job_function(void* arg)
{
    long index = (long) arg;    // armazenamos os índices das threads
	int counter = 0;            // contador para a geração aleatória das 4 letras

	while (counter < MAX_REPEAT)
    {
        // letra aleatória da letra A até Z
		char my_char = ((char)A_ASCII + (rand() % 26));

		palavra[index] = my_char;   // palavra guarda a letra gerada por cada thread
		
        pthread_barrier_wait(&barrier_palavra); // barreira espera por todas as threads
		pthread_barrier_wait(&barrier_teste);   // 
		
        counter++; // incrementamos o contador
    }
}

Noss código possui duas etapas: a geração das 4 letras e a verificação se é palíndromo

Temos 5 threads trabalhando: 4 threads geram, cada uma, uma letra; 1 thread (main) verifica se é palíndromo

Para a thread main verificar se a palavra é palíndromo, ela precisa esperar até que as outras 4 threads
tenham terminado seus jobs, ou seja, ela espera até as 4 letras serem geradas, pois a main não pode verificar
uma palavra incompleta.

Então, como a main precisa esperar a palavra estar pronta, usamos uma barreira para sincronizar as threads.

Então, a palavra é gerada e a main pode verificar se é palíndromo; usamos uma barreira para isso.
Mas se, enquanto a main verifica a palavra, as 4 threads geram uma nova palavra, a main irá verificar
uma palavra que está sendo alterada no momento de verificação e esse funcionamento não é o esperado.

Assim, temos que fazer com que as 4 threads esperem a main terminar sua verificação para poder gerar uma
nova palavra.

Para isso, então, usamos uma outra barreira.

Main aguarda job das 4 threads: usamos barreira;
4 threads aguardam job da main: usamos barreira;

Quando o funcionamento de uma thread depende do resultado do job de outra thread, é
preciso fazer uso de um atraso para que essa segunda thread obtenha a informação correta
para funcionar. Para realizar essa sincronização, usamos barreiras.

*/