#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

/*
Temos um array; vamos fazer a soma dos elementos do array;
Para tornar essa execução mais eficiente, fazemos com que
dois processos (parent e child) façam, cada um, a soma da
metade dos elementos do array (child soma do começo até a
metade do array, parent soma da metade até o fim do array);
Para o procedimento, usamos pipe para enviar o resultado da
soma do child process para o parent process, então a soma
final é executada e podemos exibir na tela o resultado da
soma dos elementos do array. (Perceba que, para um array
com uma grande quantidade de elementos, a execução desta
forma é mais eficiente, pois leva metade do tempo para ser
executada a soma).
*/

int main()
{
   int arr[] = { 1, 2, 3, 4, 1, 2, 7, 7 }; // criamos o array
    int arr_size = sizeof(arr) / sizeof(int); // quantidade de elementos do array de forma genérica
    int start, end; // primeira e última posições do array de forma genérica

    int sum = 0; // inicializamos a soma em 0

    int file_desc[2]; // file_desc[0] - saída (read); file_desc[1] - entrada (write) 
    
    if(pipe(file_desc) == -1) // se a instrução pipe(file_desc) tiver erro, retorna 1
    {
        return 1;
    }

    int id = fork();
    if(id == -1)  // se a instrução fork() tiver erro, retorna 2
    {
        return 2;
    }

    if(id == 0) // child process
    {
        // definimos o começo e fim das posições do array para child process
        start = 0;
        end = (arr_size / 2);
    }
    else
    {
        // definimos o começo e fim das posições do array para parent process
        start = (arr_size / 2);
        end = arr_size;
    }

    // executamos a soma (cada processo irá realizar a soma para seus próprios valores)
    for (int i = start; i < end; i++)
    {
        sum += arr[i];
    }
       
    // exibimos a soma
    printf("Calculated partial sum: %d\n", sum);

    if(id == 0)
    {
        // agora, vamos enviar a soma do child process para o parent
        close(file_desc[0]);

        // fazemos a escrita na parte de entrada do pipe, que levará a soma até o parent
        if( write(file_desc[1], &sum, sizeof(sum)) == -1)
        {
            // se a instrução write() tiver erro, retorna 3
            return 3;
        }
        close(file_desc[1]);
    }
    else
    {
        int sumFromChild; // a soma do child é armazenada em sumFromChild

        close(file_desc[1]);

        // fazemos a leitura na parte de saída do pipe, e parent receberá a soma
        if( read(file_desc[0], &sumFromChild, sizeof(sumFromChild)) == -1 )
        {
            // se a instrução read() tiver erro, retorna 4
            return 4;
        }
        close(file_desc[0]);

        // realizamos a soma total e exibimos na tela
        int totalSum = sum + sumFromChild;
        printf("Total sum is %d\n", totalSum);
        wait(NULL);
        /*
        Como o parent process depende do resultado do child process, usamos a instrução
        wait() para fazer o parent process esperar até o fim da execução de child process,
        para garantir que o resultado do child chegue para o parent.
        */
    }

    return 0;
}