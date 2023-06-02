// EXPLICAÇÃO DA FUNÇÃO 'FORK()'

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

int main()
{
    fork();
    fork();

    printf("Hello, world!!!\n"); // é impresso na tela 4 vezes

    /*
    Executamos o primeiro 'fork()' e passamos a ter dois processos: o processo filho e o principal
    Quando executamos o segundo 'fork()', ambos os processos já criados vão ser clonados e cada um
    cria mais um processo:

            main
            |   |
       child2   child1
                |
                child-child

    Assim, temos no total 4 processos executados.

    Para cada 'fork()' que chamamos de forma seguida e sem controle da quantidade de processos, criamos
    2^n processos para n = chamadas do fork que fazemos.
    */

    return 0;
}