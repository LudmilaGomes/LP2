#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <pthread.h>

#define SERVER_PORT 9998 // número da porta usada pelo servidor
#define BUFFER_SIZE 1024 // tamanho do buffer

/* função para retirar caracteres '\n' e '\r' da string recebida do cliente. */
void remove_caract(char *str)
{
    for (int i = 0; i < strlen(str); i++)
    {
        if (str[i] == '\n')
        {
            str[i] = '\0';
        }

        if (str[i] == '\r')
        {
            str[i] = '\0';
        }
    }
}

/* função que executa a comunicação do cliente com o servidor */
void func_client(int client_fd)
{
    /* os clientes enviam strings até que estes tenham suas conexões
    encerradas ou até que o servidor seja encerrado. */
    while (1)
    {
        // buffer_in e buffer_out são reiniciados em 0 a cada iteração para não guardar lixo
        char buffer_in[BUFFER_SIZE] = {0};
	    char buffer_out[BUFFER_SIZE] = {0};

        // lê string do usuário e salva em buffer_out
        fgets(buffer_out, BUFFER_SIZE, stdin);
        remove_caract(buffer_out); // remoção dos caracteres especiais da string recebida

        // compara string recebida com "EXIT"; se verdade, conexão do cliente é encerrada
        if (strcmp(buffer_out, "EXIT") == 0)
        {
            // envia string para o servidor
            if (send(client_fd, buffer_out, strlen(buffer_out), 0) < 0) 
            {
                perror("Erro ao enviar comando");
                exit(1);
            }

            // recebe confirmação de encerramento do servidor
            if (recv(client_fd, buffer_in, BUFFER_SIZE, 0) <= 0) 
            {
                perror("Erro ao receber confirmação");
                exit(1);
            }

            printf("Conexão encerrada.\n");
            break; // loop quebra e processo do cliente é finalizado
        }

        // compara string recebida com "SHUTDOWN"
        if (strcmp(buffer_out, "SHUTDOWN") == 0)
        {
            // envia string para o servidor
            if (send(client_fd, buffer_out, strlen(buffer_out), 0) < 0) 
            {
                perror("Erro ao enviar comando");
                exit(1);
            }

            // recebe confirmação de encerramento do servidor
            if (recv(client_fd, buffer_in, BUFFER_SIZE, 0) <= 0) 
            {
                perror("Erro ao receber confirmação");
                exit(1);
            }
            remove_caract(buffer_in); // remoção dos caracteres especiais da string recebida

            printf("Recebido: \"%s\"\n", buffer_in); // exibe mensagem de encerramento do servidor
            break; // loop quebra e processo do cliente é finalizado
        }
        
        // se as strings passam pelos testes, são strings normais enviadas para serem alteradas pelo servidor e ter seus resultados obtidos
        // envia string para o servidor
        if (send(client_fd, buffer_out, strlen(buffer_out), 0) < 0) 
        {
            perror("Erro ao enviar string");
            exit(1); 
        }

        // recebe string do servidor
        if (recv(client_fd, buffer_in, BUFFER_SIZE, 0) <= 0) 
        {
            perror("Erro ao receber resposta");
            exit(1);
        }
        remove_caract(buffer_in); // remoção dos caracteres especiais da string recebida

        // exibe a string recebida
        printf("Recebido: \"%s\"\n", buffer_in);

        // recebe string do servidor
        if (recv(client_fd, buffer_in, BUFFER_SIZE, 0) <= 0) 
        {
            perror("Erro ao receber resposta");
            exit(1);
        }

        remove_caract(buffer_in); // remoção dos caracteres especiais da string recebida

        // exibe a string recebida
        printf("Recebido: \"%s\"\n", buffer_in);
    }
}

int main()
{
    int client_fd = 0;                  // descritor do socket do cliente
	struct sockaddr_in server_address;  // salva os parâmetros e informações relativas ao socket do cliente

    // criação do socket do cliente
	client_fd = socket(AF_INET, SOCK_STREAM, 0);
	if (client_fd < 0) 
	{
		perror("socket() error!");
		exit(EXIT_FAILURE);
	}

	server_address.sin_family = AF_INET;
	int inet_pton_ret = inet_pton(AF_INET, "127.0.0.1", &server_address.sin_addr);
	if (inet_pton_ret <= 0) 
	{
		perror("inet_pton() error!");
		exit(EXIT_FAILURE);
	}
	
    // conexão do socket client_fd com endereço especificado em server_address
	server_address.sin_port = htons(SERVER_PORT);
	int con_ret = connect(client_fd, (struct sockaddr*) &server_address, sizeof(server_address));
	if (con_ret < 0) 
	{
		perror("connect() error!");
		exit(EXIT_FAILURE);
	}

    // conexão entre cliente e servidor está estabelecida

    func_client(client_fd); // executa função de comunicação

    // no fim da execução do cliente, o descritor do socket é fechado
    close(client_fd);
    return 0;
}