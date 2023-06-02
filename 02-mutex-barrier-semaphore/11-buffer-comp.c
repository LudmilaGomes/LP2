#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <semaphore.h>

#define VALUE_LIMIT 100
#define QTD_LOOP 5
#define BUFF_SIZE 10
#define QTD_PROD 1
#define QTD_CONS 5

int front = 0;
int rear = 0;
int buffer[BUFF_SIZE] = {0};

sem_t empty, full;

void* produtor(void* arg)
{
    int counter = 0;
    while(counter < BUFF_SIZE)
    {
        int value = 0;
        value = rand() % VALUE_LIMIT;
        printf("Produzi: %d\n", value);
        printf("Depositei na posicao: %d\n", rear);
        sem_wait(&empty);
        buffer[rear] = value;
        rear = (rear + 1) % BUFF_SIZE;
        sem_post(&full);
        counter++;
    }

    return 0;
}

void* consumidor(void* arg)
{
    int internal = 0;
    int counter = 0;
    while(counter < BUFF_SIZE)
    {
        sem_wait(&full);
        internal = buffer[front];
        printf("Consumi: %d\n", internal);
        printf("Retirei da posicao: %d\n", front);
        front = (front + 1) % BUFF_SIZE;
        sem_post(&empty);
        counter++;
    }

    return 0;
}

int main()
{
    time_t t;
    pthread_t prod, cons;

    srand(time(&t));

    sem_init(&empty, 0, QTD_CONS + QTD_PROD);
    sem_init(&full, 0, 0);

    pthread_create(&prod, NULL, produtor, NULL);
    pthread_create(&cons, NULL, consumidor, NULL);

    pthread_join(prod, NULL);
    pthread_join(cons, NULL);

    printf("Buffer gerado:\n");
    for (int i = 0; i < BUFF_SIZE; i++)
    {
        printf("buffer[%d] = %d\n", i, buffer[i]);   
    }

    return 0;
}