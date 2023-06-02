#include <iostream>
#include <thread>
#include <string>
#include <mutex>

using std::cout;
using std::endl;
using std::thread;
using std::string;
using std::mutex;

#define QTD_THREADS 5

mutex m;

int print_msg(string msg, int qtd) {
	for (int i = 0; i < qtd; i++) {
		m.lock();
		cout << "[" << std::this_thread::get_id();
		cout << "] " << msg << endl;
		m.unlock();
	}
	return 0;
}

int main() {
	thread* t[QTD_THREADS];

	for (int i = 0; i < QTD_THREADS; i++) {
		t[i] = new thread(print_msg, "oi brasil", i+1);
	}
	for (int i = 0; i < QTD_THREADS; i++) {
		t[i]->join();
	}

	return 0;
}
