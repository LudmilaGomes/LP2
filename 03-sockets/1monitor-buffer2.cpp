#include <iostream>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <time.h>
#include <stdlib.h>
#include <unistd.h>

#define QTD_PROD 10

using std::thread;
using std::mutex;
using std::condition_variable;
using std::cout;
using std::endl;

class monitor_buffer {
	int *buffer;
	int buffer_size;
	int count;
	int in;
	int out;
	mutex mtx;
	condition_variable not_full;
	condition_variable not_empty;
public:
	monitor_buffer(int size) {
		buffer_size = size;
		buffer = new int[size];
		count = 0;
		in = 0;
		out = 0;
	}
	int get_buffer_size() {
		return buffer_size;
	}
	void insert(int item) {
		std::unique_lock<std::mutex> lock(mtx);
		while (count == buffer_size) {
			not_full.wait(lock);
		}
		buffer[in] = item;
		in = (in + 1) % buffer_size;
		count++;
		not_empty.notify_one();
	}

	int retrieve() {
		std::unique_lock<std::mutex> lock(mtx);
		while (count == 0) {
			not_empty.wait(lock);
		}
		int item = buffer[out];
		out = (out + 1) % buffer_size;
		count--;
		not_full.notify_one();
		return item;
	}
};

monitor_buffer buf(16);

void producer() {
	for (int i = 0; i < QTD_PROD; i++) {
		int item = rand() % 100;
		buf.insert(item);
		cout << "Produziu: " << item;
		cout << endl;
		sleep(1);
	}
}

void consumer() {
	for (int i = 0; i < QTD_PROD; i++) {
		int item = buf.retrieve();
		cout << "Consumi: " << item;
		cout << endl;
		sleep(1);
	}
}

int main(void) {
	time_t t;

	srand(time(&t));

	thread prod(producer);
	thread cons(consumer);

	prod.join();
	cons.join();

	return 0;
}
