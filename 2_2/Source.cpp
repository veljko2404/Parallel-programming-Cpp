#include<iostream>
#include<thread>
#include<functional>

using namespace std;

template<typename T>
void kvadriraj_i_ispisi(T val) {
	cout << val * val << endl; 
}

template<typename T>
struct zbir {
	void operator()(T a, T b) {
		cout << a + b << endl;
	}
};

struct osoba {
	string ime;
	string prezime;
public:
	explicit osoba(string ime, string prezime) :ime(ime), prezime(prezime) {}
	void ispis() {
		cout << "Ime: " << ime << ", prezime: " << prezime << endl;
	}
};

template<typename T>
void proizvod(int a, int b, int& rezultat) {
	rezultat = a * b;
}

int main() {

	int a = 10, b = 20, c;
	osoba o("Donald","Smith");

	thread t1(kvadriraj_i_ispisi<int>, a);
	thread t2(zbir<int>(), a, b);
	thread t3(mem_fn(&osoba::ispis), o);
	thread t4(proizvod<int>, a, b, ref(c));
	thread t5([](int a, int b, int c) {
		c = a * b;
		}, a, b, ref(c));

	t1.join();
	t2.join();
	t3.join();
	t4.join();
	t5.join();
	cout << c << endl;

	return 0;
}