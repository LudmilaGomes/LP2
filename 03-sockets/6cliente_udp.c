#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <time.h>

#define SERVER_PORT 9990
#define BUFFER_SIZE 1024

int main(void) {
        int server_fd;
        struct sockaddr_in server_params;
	struct sockaddr_in recv_msg_addr;

	char buffer_in[BUFFER_SIZE] = {0};
	char buffer_out[BUFFER_SIZE] = {0};

	server_fd = socket(AF_INET, SOCK_DGRAM, 0);

        server_params.sin_family = AF_INET;

        int inet_pton_ret = inet_pton(AF_INET, "127.0.0.1",
                                &server_params.sin_addr);
        if (inet_pton_ret <= 0) {
                perror("inet_pton() error!");
                exit(EXIT_FAILURE);
        }

        server_params.sin_port = htons(SERVER_PORT);

	strcpy(buffer_out, "MENSAGEM DO CLIENTE: OLA SERVER\n");

	int ret_send = sendto(server_fd, buffer_out,
			  strlen(buffer_out), 0,
			  (struct sockaddr*) &server_params,
			  sizeof(server_params));

	if (ret_send < 0) {
		perror("sendto() error!");
		exit(EXIT_FAILURE);
	}

	printf("Enviei para o servidor: %s\n", buffer_out);

	int recv_size = 0;
	int ret_recv = recvfrom(server_fd, 
			        buffer_in, 
				BUFFER_SIZE, 0,
				(struct sockaddr*)&recv_msg_addr,
				&recv_size);

	if (ret_recv < 0) {
		perror("recvfrom() error!");
		exit(EXIT_FAILURE);
	}

	printf("Recebi do servidor: %s\n", buffer_in);

	return 0;
}
