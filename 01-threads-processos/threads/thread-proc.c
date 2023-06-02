#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>

int x = 2;

void* routine1()
{
    x++;
    sleep(2);
    printf("X = %d\n", x);
}

void* routine2()
{
    sleep(2);
    printf("X = %d\n", x);
}

/*
threads compartilham a mesma memória, e isso pode levar
a alguns erros quando duas threads acessam ao mesmo tempo
a mesma variável
*/

int main()
{
    pthread_t t1, t2;

    if(pthread_create(&t1, NULL, &routine1, NULL) != 0)
    {
        return 1;
    }

    if(pthread_create(&t2, NULL, &routine2, NULL) != 0)
    {
        return 2;
    }

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