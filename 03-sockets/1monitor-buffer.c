#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <time.h>
#include <semaphore.h>

/*
monitor que controla o acesso a um buffer compartilhado;
criar uma thread consumidora e uma thread produtora em que
ambas utilizam o monitor

buffer -> 8 chars
monitor -> inserir, recuperar

===//===

monitor que controla o acesso a um gerador de números
aleatórios;
use uma variável global para armazenar o valor gerado
e controle seu acesso com o monitor
*/

#define BUFF_SIZE 8
#define VALUE_LIMIT 30

// definição da struct
typedef struct
{
    int front;
    int rear;
    sem_t empty, full;
    int buffer[BUFF_SIZE];
} t_mon_buffer;

// definição da função que inicializa o buffer
static void init_mon_buffer(t_mon_buffer *tmb)
{
    tmb->front = 0;
    tmb->rear = 0;
    sem_init(&tmb->empty, 0, BUFF_SIZE);
    sem_init(&tmb->full, 0, 0);
}

static void mon_buffer_inserir(t_mon_buffer *tmb)
{
    int counter = 0;
    while (counter < BUFF_SIZE) {
        int value = 0;
        value = rand() % VALUE_LIMIT;
        // decrementa empty -> indica um espaço a menos disponível no buffer
        sem_wait(&tmb->empty); // aguardado para verificar se há espaço disponível no buffer
        tmb->buffer[tmb->rear] = value;
        sleep(1);
        printf("Produzi: %d\n", value);
        printf("Depositei em %d\n", tmb->rear);
        tmb->rear = (tmb->rear + 1) % BUFF_SIZE; // garantimos que o buffer é acessado em uma posição dentro do limite
        // incrementa full -> indica que há mais um elemento disponível no buffer
        sem_post(&tmb->full); // incrementado para indicar que um item foi adicionado ao buffer
        counter++;
    } 
}

static void mon_buffer_recuperar(t_mon_buffer *tmb)
{
    int internal = 0;
    int counter = 0;
    while (counter < BUFF_SIZE) {
        sem_wait(&tmb->full); // aguardado para verificar se há itens disponíveis no buffer
        internal = tmb->buffer[tmb->front];
        printf("Capturei de buffer [%d]\n", tmb->front);
        tmb->front = (tmb->front + 1) % BUFF_SIZE; // garantimos que ela permaneça dentro dos limites do buffer
        printf("Consumi: %d\n", internal);
        sem_post(&tmb->empty); // incrementado para indicar que um espaço vazio está disponível no buffer
        counter++;
    }
    return 0;
}

// funções das threads
void* producer()
{

}

void* consumer()
{
    
}

int main()
{


    return 0;
}

/*
Monitor: semelhante a uma classe; a ideia é de que a lógica do
monitor seja implementada de forma a garantir o correto funcionamento
quando do uso de threads; basicamente, nós criamos um recurso thread
safe e que pode ser utilizado sem que necessite de tratamento externo
contra consequências de concorrência.
*/