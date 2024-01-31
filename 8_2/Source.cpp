#include<iostream>
#include<omp.h>
#include<chrono>
using namespace std;
using namespace std::chrono;

template<typename T>
T scalar_product(const T* a, const T* b, int n) {
	T result = 0;
#pragma omp simd recuction(+:result) simlen(4)
	{
		for (int i = 0; i < n; i++) {
			result += a[i] * b[i];
		}
	}
	return result;
}

int main() {

	int n = 1 << 18;
	using T = double;
	T* a = new T[n];
	T* b = new T[n];

	#pragma omp parallel for
	for (int i = 0; i < n; i++) {
		a[i] = (T)rand() / RAND_MAX;
		b[i] = (T)rand() / RAND_MAX;
	}
	auto start_t = high_resolution_clock::now();
	T res = scalar_product(a, b, n);
	auto end_t = high_resolution_clock::now();

	cout << res << ", " << duration_cast<microseconds>(end_t - start_t).count();

	delete[] a;
	delete[] b;
	return 0;
}