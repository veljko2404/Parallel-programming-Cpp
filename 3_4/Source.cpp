#include<iostream>
#include<algorithm>
#include<vector>
#include<functional>
#include<thread>
#include<future>

using namespace std;

template<typename Iterator>
typename Iterator::value_type max_element_parallel(Iterator start, Iterator end, int thread_num = 8) {
	int n = distance(start, end), i;
	int work_per_thread = (n + thread_num - 1) / thread_num;
	vector<future<Iterator>> futures(thread_num);
	vector<typename Iterator::value_type> results(thread_num);
	for (i = 0; i < thread_num - 1; i++) {
		Iterator end = start;
		advance(end, work_per_thread);
		futures[i] = async(&max_element<Iterator>, start, end);
		start = end;
	}
	futures[i] = async(&max_element<Iterator>, start, end);
	for (i = 0; i < thread_num; i++) {
		results[i] = *(futures[i].get());
	}
	return *max_element(results.begin(), results.end());
}

int main() {

	int n = 10000;
	vector<int> v(n);
	for (int i = 0; i < n; i++)
		v[i] = rand() % 1000000;
	int max = max_element_parallel(v.begin(), v.end());
	int max2 = *max_element(v.begin(), v.end());
	cout << max << " " << max2;

	return 0;
}