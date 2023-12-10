#include<iostream>
#include<mutex>
#include<queue>

using namespace std;

struct queue_empty : exception {
	const char* what() { return "queue is empty"; }
};

template<typename T>
class threadsafe_queue {
	queue<T> q;
	mutex m;
public:
	bool empty() {
		lock_guard<mutex> lg(m);
		return q.empty();
	}
	void push(T value) {
		lock_guard<mutex> lg(m);
		q.push(value);
	}
	void pop(T& value) {
		lock_guard<mutex> lg(m);
		if (q.empty())throw queue_empty();
		value = move(q.front());
		q.pop();
	}
	shared_ptr<T> pop() {
		lock_guard<mutex> lg(m);
		if (q.empty())throw queue_empty();
		shared_ptr<T> ptr = make_shared<T>(move(q.front()));
		q.pop();
		return ptr;
	}
};

threadsafe_queue<int> tsq;

void producer() {
	while (true) {
		this_thread::sleep_for(chrono::milliseconds(500));
		tsq.push(rand() % 100);
	}
}

void consumer() {
	while (true) {
		this_thread::sleep_for(chrono::milliseconds(100));
		if (!tsq.empty()) {
			try {
				cout << *(tsq.pop()) << " " << this_thread::get_id() << endl;
			}
			catch (queue_empty e) {
				cout << e.what() << endl;
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