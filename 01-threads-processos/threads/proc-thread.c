#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h> 

/*

1 - um processo pode conter múltiplas threads, mas o
contrário não acontece (obviamente)

2 - há diferença no gerenciamento da memória quando
tratamos de processos e threds:

    Quando usamos fork(), as variáveis presentes no
    processo original são copiadas para o processo clonado;
    temos variáveis próprias para cada processo e as alteramos
    individual e independentemente (comunicação pode ser feita
    entre essas variáveis por meio de pipe)

    Quando criamos duas threads, por exemplo, essas duas threads
    vão acessar as mesmas variáveis (mesmo espaço de memória)

*/

int main()
{
    int x = 2;
    int pid = fork();

    if(pid == -1)
    {
        return 1;
    }

    if(pid == 0)
    {
        x++;
    }
    sleep(2);
    printf("X = %d\n", x);

    if(pid != 0)
    {
        wait(NULL);
    }

    return 0;
}