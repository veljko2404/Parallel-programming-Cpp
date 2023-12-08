#include<iostream>
#include<thread>
#include<functional>
#include<vector>
#include<numeric>
#include<algorithm>

using namespace std;

template<typename Iterator, typename T>
void accumulate_block(Iterator first, Iterator last, T& result) {
	result = accumulate(first, last, result);
}

template<typename Iterator, typename T>
T accumulate_parallel(Iterator first, Iterator last, T init, int thread_number = 8) {
	int n = distance(first, last);
	if (n == 0)return init;
	int work_per_thread = (n + thread_number - 1) / thread_number;
	vector<thread> threads(thread_number - 1);
	vector<T> results(thread_number);
	fill(results.begin(), results.end(), 0);
	int i;
	for (i = 0; i < thread_number - 1; i++) {
		Iterator end = first;
		advance(end, work_per_thread);
		threads[i] = thread(accumulate_block<Iterator, T>, first, end, ref(results[i]));
		first = end;
	}
	accumulate_block<Iterator, T>(first, last, results[i]);
	for_each(threads.begin(), threads.end(), mem_fn(&thread::join));
	return accumulate(results.begin(), results.end(), init);
}

int main() {

	vector<int> v = { 1, 2, 3, 4, 5, 0, 0, 19 };
	cout << accumulate_parallel(v.begin(), v.end(), 0) << endl;
	cout << accumulate(v.begin(), v.end(), 0);

	return 0;
}