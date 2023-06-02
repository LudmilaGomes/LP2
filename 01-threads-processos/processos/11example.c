// USO DO CÓDIGO VISTO EM SALA DE AULA (COMANDO execl())

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

#define EXEC_PATH "/home/ludmila/Documents/Linguagem-de-Programacao-2/meus-codigos/playlist/processes-c/loop"
// na dúvida, copie o caminho do código com o VSCode :')

int main() 
{
    int id = fork();

	if (id == -1) 
    {
		printf("fork() error!\n");
		exit(0);
	} 
    else 
    {
		if (id == 0) // child process
        {
			char *args[] = { "./loop", "100000", NULL };
            
			int exec_return = execv(EXEC_PATH, args);

			// esse printf() só será exibido se ocorrer algum problema com execv
			printf("[child] depois do exec(): %d\n", exec_return);
		} 
        else // parent process
        {
			int ret_val;
			waitpid(id, &ret_val, 0);
			printf("[parent] clonado finalizado!\n");
		}
	}

	return 0;
}
