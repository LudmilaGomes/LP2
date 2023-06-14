#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <pthread.h>
#include <string.h>
#include <arpa/inet.h>
#include <time.h>

/* D:
Questão (comunicação entre processos e monitores) 

Faça um servidor que atenda até 10 clientes simultâneos e realize 
a seguinte tarefa: receba uma string do cliente e responda se esta 
é um palíndromo; registre em um arquivo todas as solicitações; controle 
o acesso ao arquivo através de um monitor.
*/

#define MAX_CLIENTES 10
#define BUFFER_SIZE 1024
#define MSG_SIZE 1024
#define SERVER_PORT 9999

// declaração do monitor e suas funções
typedef struct
{
    FILE* file;
    pthread_mutex_t mutex;

} t_log_monitor;

t_log_monitor server_log; // global - visto por todas as outras threads

void write_log(t_log_monitor *log, char* msg)
{
    pthread_mutex_lock(&log->mutex);
    // escreve no arquivo de log
    printf("[log] %s\n", msg);
    pthread_mutex_unlock(&log->mutex);
}

int is_palindrome(char *str)
{
    int len = strlen(str)-2;
    int i, j;

    for (i = 0, j = len - 1; i < j; i++, j--)
    {
        if(str[i] != str[j])
            return 0;
    }
    return 1;
}

void* handle_client(void* arg)
{
    int client_socket = (int)arg;

    while (1)
    {
        char recv_buffer[BUFFER_SIZE] = {0};

        read(client_socket, recv_buffer, BUFFER_SIZE);

        if(is_palindrome(recv_buffer) == 1)
        {
            char msg[MSG_SIZE];
            sprintf(msg, "%s eh palindromo.\n", recv_buffer);
            write_log(&server_log, msg);
        }
        else
        {
            char msg[MSG_SIZE];
            sprintf(msg, "%s NAO eh palindromo.\n", recv_buffer);
            write_log(&server_log, msg);
        }
    }
}

int main()
{
    // variáveis utilizadas
    int server_socket;
    int client_socket[MAX_CLIENTES];
    int client_counter = 0;
    pthread_t client_threads[MAX_CLIENTES];
    struct sockaddr_in server_params;
    struct sockaddr_in client_params[MAX_CLIENTES];

    // criação do socket
    server_params.sin_family = AF_INET;
    server_params.sin_addr.s_addr = INADDR_ANY;
    server_params.sin_port = htons(SERVER_PORT);
    server_socket = socket(AF_INET, SOCK_STREAM, 0);
    if(server_socket < 0)
    {
        perror("socket() error!");
        write_log(&server_log, "socket() error!");
        exit(EXIT_FAILURE);
    }

    // bind - vincula o socket aos números de endereço e porta especificados em addr
    int bind_ret = bind(server_socket, (struct sockaddr *)&server_params, sizeof(server_params));
    if(bind_ret < 0)
    {
        perror("bind() error!");
        write_log(&server_log, "bind() error!");
        exit(EXIT_FAILURE);
    }

    // listen - socket em espera pela chegada de clientes
    int listen_ret = listen(server_socket, 10);
    if(listen < 0)
    {
        perror("listen() error!");
        write_log(&server_log, "listen() error!");
        exit(EXIT_FAILURE);
    }

    // accept - extrai a primeira conexão da pilha de conexões pendentes
    int client_params_size = sizeof(client_params);
    while (1) // loop infinito - o servidor aceita clientes infinitamente
    {
        client_socket[client_counter] = accept(server_socket, (struct sockaddr*) &client_params[client_counter], (socklen_t *)&client_params_size);
        if (client_socket[client_counter] < 0) 
        {
			perror("accept() error!");
            write_log(&server_log, "Accept() error!");
			exit(EXIT_FAILURE);
		}

        char msg[MSG_SIZE];
        sprintf(msg, "Novo cliente conectado %d - fd: %d\n", client_params[client_counter].sin_addr.s_addr, client_socket[client_counter]);
        write_log(&server_log, msg);
        printf(msg, "Novo cliente conectado %d - fd: %d\n", client_params[client_counter].sin_addr.s_addr, client_socket[client_counter]);

		pthread_create(&client_threads[client_counter], 0, handle_client, (void*) client_socket[client_counter]);
		client_counter = ((client_counter + 1) % 10);
    }

    return 0;
}