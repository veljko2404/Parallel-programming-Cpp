#include<iostream>
#include<thread>

using namespace std;

void write_text() {
	cout << "Nit je: " << this_thread::get_id() << endl;
}

int main() {

	thread t1(write_text);
	thread t2(write_text);

	t1.join();
	t2.join();

	return 0;
}