#include<iostream>
#include<thread>
#include<queue>
#include<chrono>
#include<mutex>

using namespace std;

queue<int> queue_data;
mutex m;
condition_variable cv;

void producer() {
	while (true) {
		this_thread::sleep_for(chrono::milliseconds(500));
		lock_guard<mutex> lg(m);
		queue_data.push(rand() % 100);
		cv.notify_one();
	}
}

void consumer_v1() {
	while (true) {
		while (queue_data.empty());
		int product = queue_data.front();
		queue_data.pop();
		cout << product << endl;
	}
}

void consumer_v2() {
	while (true) {
		while (queue_data.empty());
		unique_lock<mutex> ul(m);
		int product;
		if (!queue_data.empty()) {
			product = queue_data.front();
			queue_data.pop();
			cout << product << endl;
		}
	}
}

void consumer_v3() {
	while (true) {
		while (queue_data.empty()) {
			this_thread::sleep_for(chrono::milliseconds(100));
		}
		unique_lock<mutex> ul(m);
		int product;
		if (!queue_data.empty()) {
			product = queue_data.front();
			queue_data.pop();
			ul.unlock();
			cout << product << endl;
		}
	}
}

template<typename Consumer>
void test(Consumer consumer) {
	thread t1(producer);
	thread t2(consumer);
	thread t3(consumer);
	thread t4(consumer);

	t1.join(); t2.join(); t3.join(); t4.join();
}

void consumer_v4() {
	while (true) {
		unique_lock<mutex> ul(m);
		cv.wait(ul, []() {return !queue_data.empty(); });
		int product = queue_data.front();
		queue_data.pop();
		ul.unlock();
		cout << product << endl;
	}
}

int main() {

	//test(consumer_v1);
	//test(consumer_v2);
	//test(consumer_v3);
	test(consumer_v4);

	return 0;
}