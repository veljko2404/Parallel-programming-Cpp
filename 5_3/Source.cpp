#include<iostream>
#include<omp.h>
#include<chrono>

using namespace std::chrono;
using namespace std;
typedef long long lng;

void rand_init(int* arr, lng n) {
#pragma omp parallel for num_threads(8) shared(arr,n)
	for (lng i = 0; i < n; i++)
		arr[i] = rand() ^ 10000;
}

template<typename T>
void sum_array(T* a, T* b, T* c, lng n) {
	int num_threads = 8;
	auto start_t = high_resolution_clock::now();
#pragma omp parallel for num_threads(num_threads) \
shared(a,b,c,n)
	for (lng i = 0; i < n; i++)
		c[i] = a[i] + b[i];
	auto end_t = high_resolution_clock::now();
	cout << duration_cast<microseconds>(end_t - start_t).count() << endl;
}

int main() {

	lng n = 10000000;
	int* a = new int[n];
	int* b = new int[n];
	int* c = new int[n];

	rand_init(a, n);
	rand_init(b, n);
	rand_init(c, n);

	sum_array(a, b, c, n);

	cout << a[0] << " " << b[0] << " " << c[0];

	delete[] a;
	delete[] b;
	delete[] c;

	return 0;
}