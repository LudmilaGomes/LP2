// GET RETURN VALUE FROM A THREAD
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>

void* roll_dice()
{
    int value = (rand() % 6) + 1;

    int* result = malloc(sizeof(int));
    *result = value;

    return (void*) result;
    // passamo o valor acima para a main, mas como é uma variável alocada numa função, ela será desalocada no fim da execução da função
    // assim, temos de alocar dinamicamente esse ponteiro;
}

int main()
{
    pthread_t t;
    int* res;

    if(pthread_create(&t, NULL, roll_dice, NULL) != 0)
    {
        return 1;
    }

    if(pthread_join(t, (void**) &res) != 0)
    {
        return 2;
    }

    printf("Result: %d\n", *res);
    free(res);

    return 0;
}