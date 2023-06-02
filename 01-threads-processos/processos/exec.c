// EXPLICAÇÃO DOS COMANDOS DA FAMÍLIA exec
// 'BÔNUS': EXPLICAÇÃO DO USO DE exit() E DIFERENÇA EM RELAÇÃO AO return
// COMANDO await
// SINCRONIZAÇÃO PRODUTOR/CONSUMIDOR

/*

Da parte de threads: como funciona, diferença em relação a processos

gcc –pthread ex_pthread.c –o ex_pthread

Como criar threads em c (usando <pthread.h>), pthread_create(), pthread_join()
pthread_mutex_lock() e pthread_mutex_unlock()

*/

/*

C System Functions in Exec Family:

    int execl(const char *path, const char *arg, …, NULL);

    int execlp(const char *file, const char *arg, …, NULL );

    int execv(const char *path, char *const argv[]);

    int execvp(const char *file, char *const argv[]);

    int execle(const char *path, const char *arg, …, NULL, char * const envp[] );

    int execve(const char *file, char *const argv[], char *const envp[]);



*/