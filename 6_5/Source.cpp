#include<iostream>
#include<stack>
#include<mutex>
#include<thread>
using namespace std;

template<typename T>
class ThreadSafeStack {
	stack<T> s;
	mutex m;
	condition_variable cv;
public:
	ThreadSafeStack() = default;
	void push(T val) {
		unique_lock<mutex> ul(m);
		s.push(val);
		cv.notify_one();
	}
	T top() {
		unique_lock<mutex> ul(m);
		cv.wait(ul, [this]() {return !s.empty(); });
		return s.top();
	}
	void pop() {
		unique_lock<mutex> ul(m);
		s.pop();
	}
	bool empty() {
		return s.empty();
	}
};

ThreadSafeStack<int> tss;

void consumer() {
	while (true) {
		cout << tss.top() << endl;
		tss.pop();
	}
}

void producer() {
	while (true) {
		this_thread::sleep_for(chrono::milliseconds(500));
		tss.push(rand() % 100);
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