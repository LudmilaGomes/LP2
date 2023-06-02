#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>

// command to compile: gcc -g -pthread main.c -o main

void* routine()
{
    printf("Test from threads...\n");
    sleep(2);
    printf("Ending thread.\n");
}

int main()
{
    pthread_t t1, t2, t3;

    // pointer to the variable t1, thread's arguments, pointer to the function, argument to the function
    if(pthread_create(&t1, NULL, &routine, NULL) != 0)
    {
        return 1;
    }

    if(pthread_create(&t2, NULL, &routine, NULL) != 0)
    {
        return 2;
    }

    // says to the main to pause and wait for the end of execution of the thread
    if(pthread_join(t1, NULL) != 0)
    {
        return 3;
    }

    if(pthread_join(t2, NULL) != 0)
    {
        return 4;
    }

    return 0;
}