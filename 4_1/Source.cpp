#include<iostream>
#include<stack>
#include<mutex>

using namespace std;

struct stack_empty : exception {
	const char* what()const { return "Stack is empty!"; }
};

template<typename T>
class threadsafe_stack {
	stack<T> s;
	mutex m;
public:
	threadsafe_stack() = default;
	threadsafe_stack& operator=(const threadsafe_stack&) = delete;
	threadsafe_stack(const threadsafe_stack& other) {
		lock_guard<mutex> lg(m);
		s = other.s;
	}
	bool empty() {
		lock_guard<mutex> lg(m);
		return s.empty();
	}
	void pop(T& value) {
		lock_guard<mutex> lg(m);
		if (s.empty())throw stack_empty();
		shared_ptr<T> ptr = make_shared<T>(move(s.top()));
		s.pop();
	}
	shared_ptr<T> pop() {
		lock_guard<mutex> lg(m);
		if (s.empty())throw stack_empty();
		shared_ptr<T> ptr = make_shared<T>(move(s.top()));
		s.pop();
		return ptr;
	}
	void push(T new_value) {
		lock_guard<mutex> lg(m);
		s.push(new_value);
	}
};

threadsafe_stack<int> tss;

void producer() {
	while (true) {
		this_thread::sleep_for(chrono::milliseconds(500));
		tss.push(rand() % 100);
	}
}

void consumer() {
	while (true) {
		this_thread::sleep_for(chrono::milliseconds(100));
		if (!tss.empty()) {
			try {
				cout << *(tss.pop()) << endl;
			}
			catch (stack_empty e) {
				cout << e.what();
			}
		}
	}
}

int main() {

	thread t1(producer);
	thread t2(consumer);
	thread t3(consumer);

	t1.join();
	t2.join();
	t3.join();

	return 0;
}