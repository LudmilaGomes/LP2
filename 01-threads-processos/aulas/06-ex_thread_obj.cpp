#include <iostream>
#include <string>
#include <thread>

using std::cout;
using std::endl;
using std::string;
using std::thread;

class pessoa {
	int id;
	string nome;
	static int counter;
public:
	pessoa(int i, string n):id(i), nome(n) { counter++; }
	~pessoa() { counter--; }
	int get_id() { return id; }
	void set_id(int i ) { id = i; }
	string get_nome() { return nome; } 
	void set_nome(string n) { nome = n; } 
	void print_info(string msg) {
		cout << "pessoa(";
		cout << id << ", ";
		cout << nome << ")" << endl;
		cout << msg << endl;
	}
	static void print_counter() {
		cout << "pessoas = " << counter << endl;
	}

};

int pessoa::counter = 0;

int main() {

	pessoa p1(1001, "Pessoa Diferente");
	thread t1(&pessoa::print_info, &p1, "mensagem");
	t1.join();
	pessoa p2(1010, "Pessoa Generica");
	thread t2(pessoa::print_counter); //metodo estatico
	t2.join();


	return 0;

}
