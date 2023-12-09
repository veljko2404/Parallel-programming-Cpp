#include<iostream>
#include<mutex>
#include<thread>
#include<vector>
#include<functional>
#include<algorithm>
#include<chrono>

using namespace std;

class racun {
public:
	int id, iznos;
	mutex m;
	racun(int id, int iznos) : id(id), iznos(iznos) {}
};

void transakcija_v1(racun& sa, racun& na, int iznos) {
	this_thread::sleep_for(chrono::milliseconds(2));
	sa.iznos -= iznos;
	na.iznos += iznos;
}

void transakcija_v2(racun& sa, racun& na, int iznos) {
	lock_guard<mutex> lg1(sa.m);
	lock_guard<mutex> lg(na.m);
	this_thread::sleep_for(chrono::milliseconds(2));
	sa.iznos -= iznos;
	na.iznos += iznos;
}

void transakcija_v3(racun& sa, racun& na, int iznos) {
	lock(sa.m, na.m);
	this_thread::sleep_for(chrono::milliseconds(2));
	sa.iznos -= iznos;
	na.iznos += iznos;
	sa.m.unlock(); na.m.unlock();
}

void transakcija_v4(racun& sa, racun& na, int iznos) {
	lock(sa.m, na.m);
	lock_guard<mutex> lg1(sa.m, adopt_lock);
	lock_guard<mutex> lg2(na.m, adopt_lock);
	this_thread::sleep_for(chrono::milliseconds(2));
	sa.iznos -= iznos;
	na.iznos += iznos;
}

void transakcija_v5(racun& sa, racun& na, int iznos) {
	unique_lock<mutex> ul1(sa.m, defer_lock);
	unique_lock<mutex> ul2(na.m, defer_lock);
	lock(ul1, ul2);
	this_thread::sleep_for(chrono::milliseconds(2));
	sa.iznos -= iznos;
	na.iznos += iznos;
}

int main() {

	racun r1(1, 100000);
	racun r2(2, 0);

	vector<thread> threads(100);

	for (int i = 0; i < 100; i++)
		threads[i] = thread(transakcija_v5, ref(r1), ref(r2), 250);

	for_each(threads.begin(), threads.end(), mem_fn(&thread::join));

	cout << r1.iznos << ", " << r2.iznos;

	return 0;
}