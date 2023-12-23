#include<iostream>
#include<thread>
#include<mutex>
#include<chrono>
#include<stack>
#include<functional>
#include<algorithm>
#include<future>
using namespace std;

stack<int> s;
condition_variable cv;
mutex m;

void producer() {
	while (true) {
		this_thread::sleep_for(chrono::milliseconds(500));
		s.push(rand() % 100);
		cv.notify_one();
	}
}

void consumer() {
	while (true) {
		unique_lock<mutex> ul(m);
		cv.wait(ul, []() {return !s.empty(); });
		cout << this_thread::get_id() << ", " << s.top() << endl;
		s.pop();
	}
}

int main() {

	thread t1(producer);
	thread t2(consumer);
	thread t3(consumer);
	thread t4(consumer);

	t1.join(); t2.join(); t3.join(); t4.join();

	return 0;
}