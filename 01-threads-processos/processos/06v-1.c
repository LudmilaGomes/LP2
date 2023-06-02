// COMUNICAÇÃO ENTRE PROCESSOS USANDO PIPES

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

/*
Já vimos a comunicação entre processos mediante cópia dos
valores da memória; agora, vamos ver comunicação entre processos
com a utilização de pipes.

Pipes conectam um local a outro diretamente, assim como é 
estabelecer a comunicação entre dois processos e temos um 
processo escreve o dado de um lado, o outro processo lê do 
outro; é realmente uma conexão direta entre os processos.
Nosso pipe opera em um único sentido: entra informação só
de um lado, sai informação só de um lado

Precisamos, então, de 'file descriptors', dois valores
inteiros que vão definir a entrada-padrão e a saída-padrão
do pipe. Assim, precisamos apenas criar um array de duas
posições para armazenar esses valores, sendo o elemento 0
definindo a leitura (saída) de dados e o elemento 1 definindo
a escrita (entrada) de dados no pipe.
*/

int main()
{
    int file_desc[2]; // file_desc[0] - saída (read); file_desc[1] - entrada (write) 

    if( pipe(file_desc) == -1 )
    {
        printf("An error ocurred with opening the pipe.\n");
        return 1;
    }

    /*
    Criamos uma cópia do array 'file_desc' no processo filho, 
    e isso é importante pois podemos acessar o pipe independentemente 
    do processo. Se o processo original for encerrado, por exemplo, 
    ainda temos acesso ao pipe pelo processo filho, o que é muito útil.
    */
    int id = fork();

    if(id == -1)
    {
        printf("An error ocurred with fork.\n");
        return -1;
    }

    if(id == 0)
    {
        close(file_desc[0]);

        int x;
        printf("[child] Input a number: ");
        scanf("%d", &x);

        if( write(file_desc[1], &x, sizeof(int)) == -1)
        {
            printf("An error ocurred with writing to the pipe.\n");
            return 2;
        }
        close(file_desc[1]);
    }
    else
    {
        close(file_desc[1]);

        int y;
        
        if( read(file_desc[0], &y, sizeof(int)) == -1 )
        {
            printf("An error ocurred with reading from the pipe.\n");
            return 3;
        }
        close(file_desc[0]);

        printf("[parent] Got from child process %d\n", y);
    }

    return 0;
}