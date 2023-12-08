#include<iostream>
#include<thread>
#include<vector>
#include<functional>
#include<algorithm>

using namespace std;

template<typename T>
void sum_of_sub_array(T* arr, int from, int to, T& result) {
	for (int i = from; i < to; i++) {
		result += arr[i];
	}
}

template<typename T>
T sum_of_array(T* arr, int size, int thread_number = 8) {
	vector<thread> threads(thread_number - 1);
	vector<T> results(thread_number, 0);
	int elements_per_thread = (size + thread_number - 1) / thread_number;
	int begin = 0, end = elements_per_thread - 1, i;
	for (i = 0; i < thread_number - 1; i++) {
		threads[i] = thread(sum_of_sub_array<T>, arr, begin, end, ref(results[i]));
		begin = end;
		end += elements_per_thread;
	}
	sum_of_sub_array<T>(arr, begin, size, results[i]);
	for_each(threads.begin(), threads.end(), mem_fn(&thread::join));
	T sum = 0;
	for (T element : results) {
		sum += element;
	}
	return sum;
}

int main() {

	int n = 20;
	int* arr = new int[n];
	for (int i = 0; i < n; i++)
		arr[i] = rand() % 100;
	cout << sum_of_array<int>(arr, n, 8);
	delete[] arr;

	return 0;
}