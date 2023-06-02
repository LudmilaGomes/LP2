#include <iostream>
#include <thread>
#include <chrono>
#include <string>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

using std::thread;
using std::cout;
using std::endl;
using std::string;

int foo(int loop_limit, string msg) {
	for (int i = 0; i < loop_limit; i++) {
		cout << msg << endl;
	}
	cout << "Exibi "<< msg;
        cout << " " << loop_limit;
	cout << " vezes" << endl;
	return 0;
}

int main() {
	thread t(foo, 500, "oi brasil");

	t.join();

	return 0;

}
