#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <time.h>
#include <pthread.h>
#include <semaphore.h>

/*
(Questão 3 - 4,0)
Faça um programa em C/C++ que implemente uma solução para o problema do buffer compartilhado. 
Com um buffer compartilhado de 64 chars, o Programa deverá implementar duas funções:
 
	- produtor() - gera um caractere alfanumérico aleatório e o deposita
	no buffer; exibe o valor e a posição no buffer na tela; dorme 300ms;
	repete;
 
	- consumidor() - captura um dos valores disponíveis no buffer 
	compartilhado e armazena em um arquivo; dorme um tempo aleatório 
	entre 50ms e 200ms; repete;
 
A função produtor deverá ser realizada por uma thread; já a função consumidor deverá ser realizada 
por três (5) threads. Cada consumidor armazena as informações em um buffer diferente (de tamanho 64) 
e todas as threads devem armazenar todos os valores gerados pelo produtor. 
 
Use semáforos (biblioteca semaphore.h) para sincronizar o acesso ao buffer  compartilhado, de 
forma que todos os números gerados pelo produtor sejam consumidos uma vez, evitando qualquer 
problema ao ler ou escrever no buffer. Quando o buffer do consumidor estiver completo, o programa 
deverá exibir o seu conteúdo. Quando o buffer for consumido 2 vezes (ou seja, quando o produtor 
tiver gerado 128 caracteres e estes forem consumidos por TODOS os consumidores) finalize o programa.
*/

#define BUFFER_SIZE 64
#define QTD_CONS 5

char buffer[BUFFER_SIZE]; // buffer em que produtor salva dados
char buf_consu[QTD_CONS][BUFFER_SIZE]; // buffers para os consumidores
int front = 0, rear = 0;
sem_t empty, full, mutex; // semáforos usados
pthread_barrier_t barrier, b_consumo; // barreiras usadas
int cont_cons = 0; // contador para consumidores

/*
cont_cons é um contador usado para simular o funcionamento de um único consumidor;
cada consumidor incrementa cont_cons; quando cont_cons == QTD_THR_CONS, temos o nosso
consumidor virtual único como tendo pego o valor do buf e consumido esse valor
*/

/*

usamos as barreiras para sincronizar o processo de produzir e 
guardar o valor em buf e de consumir o valor de buf

etapa 1 - gerar dado e armazenar em buf
etapa 2 - consumidores consumirem buf
etapa 3 - após o consumo ter sido finalizado, o produtor pode produzir de novo

*/

// não tive tempo de comentar melhor

void* funcao_produtor(void* p)
{
    int gerado = 0;
    while (1) 
    {
        char c = 'A' + rand() % 26;
        sem_wait(&empty);
        buffer[rear] = c;
        rear = (rear + 1) % BUFFER_SIZE;
        printf("[Produtor] gerado: %c [%d]\n", c, rear);

        gerado++;
        usleep(300);
        sem_post(&full);

        pthread_barrier_wait(&barrier);

        printf("gerado = %d\n", gerado);
        if (gerado == BUFFER_SIZE - 1)
        {
            rear = 0;
        }

        if (gerado == 128) 
        {
            printf("[P] Fim do produtor\n");
            pthread_exit(0);
        }

        pthread_barrier_wait(&b_consumo);
    }
    return NULL;
}

void* funcao_consumidor(void* p)
{
    int cont_posicao = 0;
    long id = (long)p;
    while (1)
    {
        pthread_barrier_wait(&barrier);
            
        sem_wait(&mutex);
        cont_cons++;
        sem_post(&mutex);

        if(cont_cons == QTD_CONS)
            sem_wait(&full);

        buf_consu[id][cont_posicao] = buffer[front];
        
        int dormir = 50 + rand() % 151;
        sem_wait(&mutex);
        
        printf("cont_posicao = %d\n", cont_posicao);
        if (cont_posicao == BUFFER_SIZE - 1)
        {
            printf("Buffer do consumidor %d: \n", id);
            for (int i = 0; i < BUFFER_SIZE; i++)
            {
                printf("%c ", buf_consu[id][i]);
            }
            printf("\n");
            cont_posicao = 0;
        }
        sem_post(&mutex);
        usleep(dormir);

        sem_wait(&mutex);
        cont_cons--;
        sem_post(&mutex);
        
        printf("[%d] cont_cons = %d\n", id, cont_cons);
        if(cont_cons == 0)
        {
            printf("dentro do if(cont_cons == 0)\n");
            sem_post(&full);
            front = (front + 1) % BUFFER_SIZE;
        }
        cont_posicao++;
        pthread_barrier_wait(&b_consumo);
    }

    return NULL;
}

int main(void) {
    pthread_t produtor;
    pthread_t consumidores[QTD_CONS];
    pthread_barrier_init(&barrier, 0, QTD_CONS + 1);
    pthread_barrier_init(&b_consumo, 0, QTD_CONS + 1);
    time_t t;

    srand((unsigned) time(&t));

    sem_init(&empty, 0, BUFFER_SIZE);
    sem_init(&full, 0, 0);
    sem_init(&mutex, 0, 1);

    pthread_create(&produtor, NULL, funcao_produtor, NULL);

    for (long i = 0; i < QTD_CONS; i++) 
    {
        pthread_create(&consumidores[i], NULL, funcao_consumidor, (void*)i);
    }

    pthread_join(produtor, NULL);

    for (int i = 0; i < QTD_CONS; i++) 
    {
        pthread_join(consumidores[i], NULL);
    }

    return 0;
}