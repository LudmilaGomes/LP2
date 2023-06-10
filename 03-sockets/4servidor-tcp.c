// implementação de servidor usando socket - teste realizado com telnet ($telnet localhost 9999)
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <pthread.h>
#include <string.h>
#include <arpa/inet.h>
#include <time.h>

/*

servidor - o que é necessário

1- criação do socket: definimos socket e seus valores
2- bind:              vincula o socket aos números de endereço e porta especificados em addr
3- listen:            põe o socket do servidor em espera pela chegada do cliente no servidor para fazer a conexão
4- accept:            extrai a primeira conexão da pilha de conexões pendentes para o socket

*/

#define PORT 9999
#define BUFFER_SIZE 1024
#define BACKLOG_SIZE 5
#define QTD_CLIENTES 10

// alteração para conseguir tratar mais de um cliente usando threads
// uma thread é criada para cada cliente
void* client_handling(void* arg)
{
    int my_client_fd = (int)arg;
    char buffer_in[BUFFER_SIZE] = {0};
    char buffer_out[BUFFER_SIZE] = {0};

    strcpy(buffer_out, "BEM VINDO AO SERVIDOR\n");

	write(my_client_fd, buffer_out, strlen(buffer_out));
	printf("Enviei ao cliente (fd: %d): %s\n", my_client_fd,
											buffer_out);

	int ret_read = read(my_client_fd, buffer_in, BUFFER_SIZE);
	if (ret_read < 0) {
			perror("read() error!");
			exit(EXIT_FAILURE);
	}

	printf("Recebi do cliente (fd: %d): %s\n", my_client_fd,
					buffer_in);

	if (strstr(buffer_in, "RAND") != NULL) {
		int rnd = rand() % 1000;
		sprintf(buffer_out, "RANDOM: %d\n", rnd);
	} else {
	        strcpy(buffer_out, "TCHAU\n");
	}

        write(my_client_fd, buffer_out, strlen(buffer_out));
        printf("Enviei ao cliente (fd: %d): %s\n", my_client_fd,
						buffer_out);

	close(my_client_fd);

	return 0;
}

int main()
{
    int server_fd;
    int client_fd[QTD_CLIENTES];
    struct sockaddr_in server_params;
    struct sockaddr_in client_addr[QTD_CLIENTES];

    pthread_t cli_threads[QTD_CLIENTES];

    int client_count = 0;

    time_t t;
	srand(time(&t));

    server_fd = socket(AF_INET, SOCK_STREAM, 0); // TCP
    if(server_fd  < 0)
    {
        perror('socket() error!');
        exit(EXIT_FAILURE);
    }

    server_params.sin_family = AF_INET;            // TCP-IP
    server_params.sin_addr.s_addr = INADDR_ANY;    // SERVIDOR
    server_params.sin_port = htons(PORT);          // PORT

    int ret_bind = bind(server_fd, (struct sockaddr*)&server_params, sizeof(server_params)); // vincula o socket criado aos parâmetros definidos
    if(ret_bind < 0)
    {
        perror('bind() error!');
        exit(EXIT_FAILURE);
    }

    int ret_list = listen(server_fd, BACKLOG_SIZE);
    if(ret_list < 0)
    {
        perror('listen() error!');
        exit(EXIT_FAILURE);
    }

    printf("Servidor aguardando conexoes na porte %d...\n", PORT);

    int client_addr_size = sizeof(client_addr);
	while (1) 
    {
		client_fd[client_count] = accept(server_fd, (struct sockaddr*) &client_addr[client_count], (socklen_t *)&client_addr_size);

		if (client_fd[client_count] < 0) 
        {
			perror("accept() error!");
			exit(EXIT_FAILURE);
		}

		printf("Novo cliente conectado %d - fd: %d\n", client_addr[client_count].sin_addr.s_addr, client_fd[client_count]);

		pthread_create(&cli_threads[client_count], 0, client_handling, (void*) client_fd[client_count]);
		client_count = ((client_count + 1) % 10);
	}

    return 0;
}