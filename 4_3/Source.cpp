#include<iostream>
#include<vector>
#include<algorithm>
#include<functional>
#include<numeric>
#include<atomic>
#include<thread>
#include<float.h>

using namespace std;

template<typename Iterator, typename T>
void accumulate_block(Iterator start, Iterator end, atomic<T>& overall_result) {
	T result = accumulate(start, end, 0, [](T acc, T new_value) {
		return acc + new_value * new_value;
		});
	overall_result += result;
}

template<typename Iterator, typename T>
T euclid_norm_parallel(Iterator start, Iterator end, T initial_value = 0, int thread_num = 8) {
	int n = distance(start, end);
	int work_per_thread = (n + thread_num - 1) / thread_num;
	atomic<T> result = 0;
	vector<thread> threads(thread_num - 1);
	for (int i = 0; i < thread_num - 1; i++) {
		Iterator end = start;
		advance(end, work_per_thread);
		threads[i] = thread(accumulate_block<Iterator, float>, start, end, ref(result));
		start = end;
	}
	accumulate_block(start, end, result);
	for_each(threads.begin(), threads.end(), mem_fn(&thread::join));
	return sqrt(result);
}

int main() {

	int n = 1000;
	vector<int> v(n);
	for (int i = 0; i < n; i++)
		v[i] = rand() % 20;

	float r1 = euclid_norm_parallel(v.begin(), v.end(), 0.0f, 1);
	float r2 = euclid_norm_parallel(v.begin(), v.end(), 0.0f);
	cout << r1 << endl;
	cout << r2 << endl;

	return 0;
}