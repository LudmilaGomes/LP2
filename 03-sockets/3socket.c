#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <sys/socket.h>

/*

servidor - o que é necessário

1- criação do socket: definimos socket e seus valores
2- bind:              vincula o socket aos números de endereço e porta especificados em addr
3- listen:            põe o socket do servidor em espera pela chegada do cliente no servidor para fazer a conexão
4- accept:            extrai a primeira conexão da pilha de conexões pendentes para o socket

*/

int main()
{
    int server_fd, new_socket;
    struct sockaddr_in endereco;
    server_fd = socket(AF_INET, SOCK_STREAM, 0);

    endereco.sin_family = AF_INET;
    endereco.sin_addr.s_addr = INADDR_ANY;
    endereco.sin_port = htons(8000);

    // bind
    // listen
    // accept

    return 0;
}