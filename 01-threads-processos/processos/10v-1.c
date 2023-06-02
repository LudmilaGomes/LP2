// USO DE 2 PIPES

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <sys/wait.h>

/*
Eu tenho um processo1 com um valor 5;
O processo1 envia esse valor 5 para um processo2;
O processo2 multiplica 5 por 4;
O processo2 devolve o resultado (20) para o processo1;
O processo1 exibe o resultado na tela;

Quando usamos somente 1 pipe, erros podem acontecer,
como o parent process ler dele mesmo. Então, para que
esse programa funcione, precisamos usar 2 pipes.
*/

int main()
{
    int file_desc1[2];
    int file_desc2[2];

    if(pipe(file_desc1) == -1)
    {
        printf("Error pipe.\n");
        return 1;
    }

    if(pipe(file_desc2) == -1)
    {
        printf("Error pipe.\n");
        return 1;
    }

    int id = fork();
    if(id == -1)
    {
        printf("Error fork.\n");
        return 2;
    }

    if(id == 0)
    {
        // child process

        close(file_desc2[0]);
        close(file_desc1[1]);

        int x;
        if( read(file_desc1[0], &x, sizeof(x)) == -1 )
        {
            printf("Error read (pipe).\n");
            return 3;
        }
        printf("[child] Received %d\n", x);

        x = x * 4;

        if( write(file_desc2[1], &x, sizeof(x)) == -1 )
        {
            printf("Error write (pipe) 4.\n");
            return 4;
        }
        printf("[child] Wrote %d\n", x);
        
        close(file_desc1[0]);
        close(file_desc2[1]);
    }
    else
    {
        close(file_desc2[1]);
        close(file_desc1[0]);
        // parent process
        srand(time(NULL));
        int y = ((rand() % 10) + 1); // numbers from 0 to 9

        if( write(file_desc1[1], &y, sizeof(y)) == -1 )
        {
            printf("Error write (pipe) 5.\n");
            return 5;
        }
        printf("[parent] Wrote %d\n", y);

        int final_result;

        if( read(file_desc2[0], &final_result, sizeof(final_result)) == -1 )
        {
            printf("Error read (pipe).\n");
            return 6;
        }
        printf("[parent] Result %d\n", final_result);
        wait(NULL);

        close(file_desc2[0]);
        close(file_desc1[1]);
    }

    return 0;
}