#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <pthread.h>

#define SHTDWN_CLIENTS 3    // quantidade de clientes para encerramento do servidor
#define MAX_CLIENTS 5       // quantidade máxima de clientes simultâneos
#define BUFFER_SIZE 1024    // tamanho do buffer
#define MSG_SIZE 1024       // tamanho da mensagem que é armazenada no arquivo de log
#define SERVER_PORT 9998    // número da porta usada pelo servidor

/* declaração do monitor, usado para:
1- escrever em um arquivo de log garantindo que o arquivo seja acessado 
por uma só thread (através do mutex);
2- controlar o encerramento do servidor (através do mutex, barrier e 'count_shutdown'.
* count_shutdown conta quantidade de clientes que mandaram 'SHUTDOWN'; é incrementada
e verificada para o encerramento do servidor;
* barrier é usada para que os clientes que enviaram 'SHUTDOWN' esperem pelos outros; 
assim, a execução deles fica bloqueada até que a quantidade certa de clientes tenha
chegado; a execução é retomada, o servidor é encerrado, assim como os clientes. */
typedef struct
{
    FILE* file;
    pthread_mutex_t mutex;
    pthread_barrier_t barrier;
    int count_shutdown;

} t_log_monitor;

/* monitor server_log global para ser acessado pela função test_shtdwn e por 
handle_client; server_log é visto e acessado por todas as threads; server_socket 
global para ser acessado pela função test_shtdwn. */
t_log_monitor server_log;
int server_socket;

/* função para escrever informações no arquivo de log (através de objeto do tipo t_log_monitor). */
void write_log(t_log_monitor *log_mon, char* msg)
{
    /* clientes simultâneos podem acessar o servidor, então, para garantir que as
    threads não vão acessar e alterar o arquivo de log ao mesmo tempo, usamos mutex. */
    pthread_mutex_lock(&log_mon->mutex);
    // início - seção crítica
    /* modo de abertura "a" permite escrever novos dados no fim do arquivo; se o arquivo não existe, ele é criado. */    
    log_mon->file = fopen("arquivo.txt", "a");  // abre arquivo
    fprintf(log_mon->file, "[log] %s", msg);
    fclose(log_mon->file);                      // fecha arquivo
    // fim - seção crítica
    pthread_mutex_unlock(&log_mon->mutex);
}

/* inicializa o count_shutdown e barrier do monitor para que sejam utilizados e funcionem corretamente */
void init_count(t_log_monitor *log_mon)
{
    log_mon->count_shutdown = 0;
    pthread_barrier_init(&log_mon->barrier, 0, SHTDWN_CLIENTS);
}

/* função responsável por fazer controle do encerramento do servidor a partir 
do uso do monitor; a variável count_shutdown é incrementada quando as string
str recebida e "SHUTDOWN" são iguais; count_shutdown é verificada com o valor
'>= SHTDWN_CLIENTS' e, se a quantidade for atingida pelo contador, o servidor
envia mensagens de finalização para os clientes, arquivo de log e é encerrado. */
void test_shtdwn(t_log_monitor *log_mon, char *str, int client_socket)
{
    // compara str e "SHUTDOWN"; se verdade, incrementa count_shutdown
    if (strcmp(str, "SHUTDOWN") == 0)
    {
        pthread_mutex_lock(&log_mon->mutex);
        log_mon->count_shutdown++;
        pthread_mutex_unlock(&log_mon->mutex);

        pthread_barrier_wait(&log_mon->barrier);
    }
    
    // verifica count_shutdown; se verdade, servidor é finalizado
    if (log_mon->count_shutdown >= SHTDWN_CLIENTS)
    {
        char msg[MSG_SIZE] = {0};

        printf("[cliente %d] SERVIDOR ENCERRANDO (%s)...\n", client_socket, str);

        // uso de mutex para garantir que as threads não alteram msg simultaneamente
        pthread_mutex_lock(&log_mon->mutex);
        sprintf(msg, "[cliente %d] SERVIDOR ENCERRANDO (%s)...\n", client_socket, str);
        pthread_mutex_unlock(&log_mon->mutex);

        write_log(&server_log, msg);

        if (send(client_socket, msg, BUFFER_SIZE, 0) < 0) 
        {
            perror("Erro ao enviar string");
            write_log(&server_log, "send() error!");
            exit(1);
        }

        // fecha socket do cliente
        close(client_socket);
        pthread_barrier_wait(&log_mon->barrier);

        // encerra socket do servidor; SHUT_RDWR indica que o servidor não recebe nem transmite dados
        shutdown(server_socket, SHUT_RDWR);
        exit(0);
    }
}

/* função para substituir letras minúsculas por maiúsculas; a string recebida do 
cliente é percorrida; quando letras minúsculas são encontradas, são substituídas
pela maiúscula. */
void upper(char *str)
{
    int len = strlen(str);
    int diff = abs('A' - 'a'); // guarda módulo da diferença entre 'A' e 'a'

    for (int i = 0; i < len; i++)
    {
        if(str[i] >= 'a' && str[i] <= 'z')
        {
            /* na tabela ASCII, letras maiúsculas têm valor menor que minúsculas e,
            para substituir minúscula por maiúscula, basta subtrair diff da minúscula;
            diff é a distância que separa a minúscula de sua equivalente maiúscula. */
            str[i] = str[i] - diff;
        }
    }
}

/* função para retirar caracteres '\n' e '\r' da string recebida do cliente. A string 
é percorrida até seu tamanho - 1; quando '\n' e '\r' são encontrados, eles são
substituídos pelo caractere finalizador de string '\0'. Assim, as comparações 
da string com "EXIT" e "SHUTDOWN" serão realizadas sem erro. */
void remove_caract(char *str)
{
    for (int i = 0; i < (strlen(str) - 1); i++)
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

/* função executada pelas threads (estas 'atendem' cada cliente); 
executa a comunicação do servidor com o cliente */
void* handle_client(void* arg)
{
    // client_socket é passado para a função e usado por ela para comunicação e em close()
    int client_socket = (int)arg;

    /* os clientes enviam strings até que estes tenham suas conexões
    encerradas ou até que o servidor seja encerrado. */
    while (1)
    {
        // buffer e msg são reiniciados em 0 a cada iteração para não guardar lixo
        char recv_buffer[BUFFER_SIZE] = {0};
        char msg[MSG_SIZE] = {0};

        // recebe a string do cliente
        if (recv(client_socket, recv_buffer, BUFFER_SIZE, 0) <= 0) 
        {
            // se ocorrer um erro, o servidor é encerrado; erro é escrito no arquivo de log
            perror("Erro ao receber resposta");
            write_log(&server_log, "recv() error!");
            exit(1);
        }
        remove_caract(recv_buffer); // remoção dos caracteres especiais da string recebida

        // compara string recebida com "EXIT"; se verdade, conexão do cliente é encerrada
        if (strcmp(recv_buffer, "EXIT") == 0)
        {
            // exibe mensagens para servidor e salva no arquivo de log
            printf("[cliente %d] CONEXAO DO CLIENTE ENCERRADA (STRING RECEBIDA: %s)\n", client_socket, recv_buffer);
            sprintf(msg, "[cliente %d] CONEXAO DO CLIENTE ENCERRADA (STRING RECEBIDA: %s)\n", client_socket, recv_buffer);
            write_log(&server_log, msg);

            // envia a string msg para o cliente
            if (send(client_socket, msg, BUFFER_SIZE, 0) < 0) 
            {
                perror("Erro ao enviar string");
                write_log(&server_log, "send() error!");
                exit(1);
            }

            // fecha socket do cliente
            close(client_socket);
            return;
        }
        
        // chama a função para verificar string e verificar se o contador chegou ao número limite de mensagens "SHUTDOWN"
        test_shtdwn(&server_log, recv_buffer, client_socket);

        // se as strings passam pelos testes, são strings normais obtidas para serem alteradas pelo servidor e ter seus resultados enviados de volta
        // exibe mensagens para servidor e salva no arquivo de log
        printf("[cliente %d] STRING RECEBIDA: %s\n", client_socket, recv_buffer);
        sprintf(msg, "[cliente %d] STRING RECEBIDA: %s\n", client_socket, recv_buffer);
        write_log(&server_log, msg);
        // envia a string msg para o cliente
        if (send(client_socket, msg, BUFFER_SIZE, 0) < 0) 
        {
            perror("Erro ao enviar string");
            write_log(&server_log, "send() error!");
            exit(1);
        }

        // chama a função que substitui letras minúsculas por maiúsculas
        upper(recv_buffer);

        // exibe mensagens para servidor e salva no arquivo de log
        printf("[cliente %d] STRING RESULTANTE: %s\n", client_socket, recv_buffer);
        sprintf(msg, "[cliente %d] STRING RESULTANTE: %s\n", client_socket, recv_buffer);
        write_log(&server_log, msg);
        // envia a string msg para o cliente
        if (send(client_socket, msg, BUFFER_SIZE, 0) < 0) 
        {
            perror("Erro ao enviar string");
            write_log(&server_log, "send() error!");
            exit(1);
        }
    }
}

int main()
{
    // variáveis utilizadas
    int client_socket[MAX_CLIENTS];                 // descritores dos sockets dos clientes
    int client_counter = 0;                         // descritor do socket do servidor
    pthread_t client_threads[MAX_CLIENTS];          // tipo thread para atender os clientes simultaneamente
    struct sockaddr_in server_params;               // salva os parâmetros e informações relativas ao socket do servidor
    struct sockaddr_in client_params[MAX_CLIENTS];  // salva os parâmetros e informações relativas aos sockets dos clientes

    init_count(&server_log); // inicializa campos do objeto do monitor;

    // criação do socket - salvamos informações como address family e número da porta
    server_params.sin_family = AF_INET;
    server_params.sin_addr.s_addr = INADDR_ANY;
    server_params.sin_port = htons(SERVER_PORT);

    /* cria socket com as informações dadas; server_socket é o descritor de socket; usamos AF_INET 
    para comunicação entre processos conectados em hosts diferentes por IPV4; SOCK_STREAM indica o
    tipo da comunicação (que, nesse caso, é TCP); 0 é valor de protocolo para IP. */
    server_socket = socket(AF_INET, SOCK_STREAM, 0);
    if(server_socket < 0) // caso a criação do socket tenha algum erro (retorna número negativo)
    {
        perror("socket() error!");
        write_log(&server_log, "socket() error!"); // escreve no arquivo de log que houve um erro
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

    /* listen - socket em espera pela chegada de clientes; o segundo parâmetro define
    o máximo que a pilha de conexões pendentes pode crescer. */
    int listen_ret = listen(server_socket, (MAX_CLIENTS + 5));
    if(listen < 0)
    {
        perror("listen() error!");
        write_log(&server_log, "listen() error!");
        exit(EXIT_FAILURE);
    }

    int client_params_size = sizeof(client_params);
    
    char msg[MSG_SIZE];
    sprintf(msg, "Pronto para receber conexoes na porta %d...\n", SERVER_PORT);
    write_log(&server_log, msg);
    printf("Pronto para receber conexoes na porta %d...\n", SERVER_PORT);
    
    // loop infinito - o servidor aceita clientes infinitamente (até que seja encerrado)
    while (1)
    {
        /* accept - extrai a primeira conexão da pilha de conexões pendentes para o socket do servidor;
        o retorno, que é um novo descritor de arquivo (referente ao novo socket criado) é salvo em client_socket
        na posição 'client_counter' */
        client_socket[client_counter] = accept(server_socket, (struct sockaddr*) &client_params[client_counter], (socklen_t *)&client_params_size);
        if (client_socket[client_counter] < 0) 
        {
			perror("accept() error!");
            write_log(&server_log, "Accept() error!");
			exit(EXIT_FAILURE);
		}

        // conexão entre cliente e servidor está estabelecida; estes podem se comunicar

        char msg[MSG_SIZE] = {0};
        sprintf(msg, "Novo cliente conectado %d - fd: %d\n", client_params[client_counter].sin_addr.s_addr, client_socket[client_counter]);
        write_log(&server_log, msg);
        printf("Novo cliente conectado %d - fd: %d\n", client_params[client_counter].sin_addr.s_addr, client_socket[client_counter]);

        // cada cliente é 'atendido' por uma thread, que é criada neste ponto do código e executa a função handle_client()
		pthread_create(&client_threads[client_counter], 0, handle_client, (void*) client_socket[client_counter]);
		
        // client_counter é incrementada de forma que seu número nunca passa do limite de clientes máximo
        client_counter = ((client_counter + 1) % MAX_CLIENTS);
    }

    return 0;
}