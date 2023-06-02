#include <iostream>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <time.h>
#include <unistd.h>

#define BUFFER_SIZE 8
#define QTD_PROD 10

using namespace std;

class monitor_buffer 
{
    private:
        int buffer[BUFFER_SIZE];
        int count;
        int in;
        int out;
        mutex mtx;
        // condition_variable funciona como semáforo
        condition_variable not_full;
        condition_variable not_empty;

    public:
        monitor_buffer()
        {
            count = 0;
            in = 0;
            out = 0;
        }

        void inserir(int item)
        {
            unique_lock lock(mtx);

            while (count == BUFFER_SIZE)
            {
                not_full.wait(lock);
            }
            buffer[in] = item;
            in = (in + 1) % BUFFER_SIZE;
            count++;
            not_empty.notify_one();
        }

        int recuperar()
        {
            unique_lock lock(mtx);
            while (count == 0)
            {
                not_empty.wait(lock);
            }
            int item = buffer[out];
            out = (out + 1) % BUFFER_SIZE;
            count--;
            not_full.notify_one();
            return item;
        }
};

monitor_buffer buf;

// poderíamos também passar como parâmetro o monitor criado para as funções abaixo;
void producer()
{
    for (int i = 0; i < QTD_PROD; i++)
    {
        int item = rand() % 100;
        buf.inserir(item);
        printf("Produtor produziu: %d\n", item);
        sleep(1);
    }
}

void consumer()
{
    for (int i = 0; i < QTD_PROD; i++)
    {
        int item = buf.recuperar();
        printf("Consumidor consumiu: %d\n", item);
        sleep(1);
    }
}

int main()
{
    thread prod(producer);
    thread cons(consumer);
    time_t t;

    srand(time(&t));

    prod.join();
    cons.join();

    return 0;
}