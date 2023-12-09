#include<iostream>
#include<vector>
#include<thread>
#include<algorithm>
#include<functional>
#include<mutex>
#include<chrono>

using namespace std;

mutex m;

void add_one(int& x) {
	this_thread::sleep_for(chrono::milliseconds(1));
	m.lock();
	x++;
	m.unlock();
}

void add_one_v2(int& x) {
	this_thread::sleep_for(chrono::milliseconds(1));
	lock_guard<mutex> lg(m);
	x++;
}

void increment(int &x) {
	int n = 1000, i;
	vector<thread> threads(1000);
	for (i = 0; i < n; i++) {
		//threads[i] = thread(add_one, ref(x));
		threads[i] = thread(add_one_v2, ref(x));
	}
	for_each(threads.begin(), threads.end(), mem_fn(&thread::join));
}

int main() {

	int x = 0;
	increment(x);
	cout << x << endl;

	return 0;
}