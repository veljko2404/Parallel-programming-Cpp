#include<iostream>
#include<mutex>
#include<thread>
#include<functional>
#include<queue>

using namespace std;

template<typename T>
class threadsafe_queue {
	queue<T> q;
	mutex m;
	condition_variable cv;
public:
	threadsafe_queue() = default;
	threadsafe_queue& operator=(const threadsafe_queue& other) = delete;
	bool empty() {
		lock_guard<mutex> lg(m);
		return q.empty();
	}
	void push(T val) {
		lock_guard<mutex> lg(m);
		q.push(val);
		cv.notify_one();
	}
	shared_ptr<T> pop() {
		lock_guard<mutex> lg(m);
		if (q.empty()) return shared_ptr<T>();
		shared_ptr<T> ptr = make_shared<T>(move(q.front()));
		q.pop();
		return ptr;
	}
	shared_ptr<T> wait_and_pop() {
		unique_lock<mutex> ul(m);
		cv.wait(ul, [this]() {return !q.empty(); });
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
		int num = *(tsq.wait_and_pop());
		cout << num << endl;
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