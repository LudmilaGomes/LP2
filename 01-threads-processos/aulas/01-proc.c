#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main()
{
	int r = fork();
	
	if(r == 0) // processo clonado
	{
		printf("CLONADO\n");
		printf("meu PID: %d\n", getpid());
		printf("PID do meu processo original: %d\n", getppid());
	}
	else // processo original
	{
		printf("ORIGINAL\n");
		printf("meu PID: %d\n", getpid());
		printf("PID do meu processo original: %d\n", getppid());
	}	
	
	
	/*
	printf("ANTES\n");
	
	int r = fork();
	
	// valor de retorno de fork() == 0: processo clonado
	if(r == 0)
	{
		printf("clonado, PID: [%d]\n", getpid());
		//while(1);
	}	
	else
	{
		printf("original, PID: [%d]\n", getpid());
		printf("PID do processo clonado: %d\n", r);
	}
	*/

	/*
	printf("ANTES\n");
	
	int r = fork();
	
	// valor de retorno de fork() == 0: processo clonado
	if(r == 0)
	{
		printf("clonado\n");
		while(1);
	}	
	else
	{
		printf("original\n");
		printf("PID do processo clonado: %d\n", r);
	}
	*/

	/*
	printf("ANTES\n");
	fork();
	printf("DEPOIS\n");
	
	while(1);
	
	return 0;
	*/

	/*
	int x;
	
	printf("Alo, mundo\n");
	
	sleep(10);
	
	printf("Digite um valor para X: ");
	scanf("%d", &x);
	
	printf("X = [%d]", x);
	*/
	
	/*
	for(int i = 0; i < 1000; i++)
	{
		printf("alo [%d]\n", i);
	}
	*/
	
	/*
	int i = 0;
	
	while(1)
	{
		printf("alo [%d]\n", i);
		i++;
	}
	*/
	
	return 0;
}
