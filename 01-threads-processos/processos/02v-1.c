// EXPLICAÇÃO DO USO DA FUNÇÃO 'WAIT()'

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
//#include <wait.h>

int main()
{
    int id = fork();
    int n;

    if(id == 0) // child process
    {
        n = 1;
    }
    else
    {
        n = 6;
    }

    // então, dizemos para a main esperar: 
    if(id != 0)
    {
        wait();
    }

    for (int i = n; i < n + 5; i++)
    {
        printf("%d ", i);
        fflush(stdout);
    }

    if(id != 0)
    {
        printf("\n");
    }

    return 0;
}

/*
    A saída esperada seria algo como: 
        1 6 2 7 3 8 4 9 5 10  

    Ou:  
        6 7 8 9 10 
        1 2 3 4 5 

    E queremos que apareça dessa forma:

        1 2 3 4 5 
        6 7 8 9 10
    
    Isso acontece porque os processos estão sendo executados simultaneamente e os
    resultados de n são impressos 'quase' ao mesmo tempo como da forma mostrada
    acima, ou seja, se intercalando (já que, justamente, os processos são concorrentes).

    O que fazer para isso não acontecer e um processo esperar até o fim do outro? Como
    podemos fazer com que a main espere pelo processo filho ser executado primeiro para
    depois executar?

    Temos de parar a execução do processo principal até que a execução do processo filho
    seja concluído; podemos indicar isso ao programa usando a instrução 'wait()'.
*/ 