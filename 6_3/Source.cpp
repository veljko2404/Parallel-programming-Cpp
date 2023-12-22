#include<iostream>
#include<thread>
#include<vector>
#include<functional>
#include<algorithm>
using namespace std;

template<typename T>
void part_sum(T* arr1, T* arr2, T* arr3, int begin, int end) {
	for (int i = begin; i < end; i++) {
		arr3[i] = arr1[i] * arr2[i];
	}
}

template<typename T>
void sum(T* a, T* b, T* c, int n, int thread_number) {
	vector<thread> threads(thread_number - 1);
	int part = (n + thread_number - 1) / thread_number;
	int start = 0, end = part;
	for (int i = 0; i < thread_number - 1; i++) {
		threads[i] = thread(part_sum<T>, a, b, c, start, end);
		start = end + 1;
		end += part;
	}
	part_sum(a, b, c, start, n);
	for_each(threads.begin(), threads.end(), mem_fn(&thread::join));
}

int main() {

	int n = 100;
	int* a = new int[n];
	int* b = new int[n];
	int* c = new int[n];

	for (int i = 0; i < n; i++) {
		a[i] = rand() % 20;
		b[i] = rand() % 20;
		c[i] = rand() % 20;
	}

	sum<int>(a, b, c, n, 8);

	for (int i = 0; i < n; i++)
		cout << i << ". " << a[i] << " * " << b[i] << " = " << c[i] << endl;

	delete[] a;
	delete[] b;
	delete[] c;
	return 0;
}