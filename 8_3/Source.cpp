#include<iostream>
#include<omp.h>
#include<chrono>
using namespace std;
using namespace std::chrono;

template<typename T>
void scalar_product(const T* a, const T* b, int n, T& result_store) {
	T result = 0;
	#pragma omp simd reduction(+:result) simdlen(4)
	{
		for (int i = 0; i < n; i++)
			result += a[i] * b[i];
	}
	result_store = result;
}

template<typename T>
void mat_vec_mul(T** mat,T* x, T* y,int n, int m) {
#pragma omp parellel for num_threads(8)
	for (int i = 0; i < n; i++)
		scalar_product(mat[i], x, m, y[i]);
}

int main() {

	int n = 100;
	using T = double;
	T** matrix = new T * [n];
	for (int i = 0; i < n; i++)
		matrix[i] = new T[n];

	T* x = new T[n];
	T* y = new T[n];
	#pragma omp parallel for num_threads(8)
	for (int i = 0; i < n; i++) {
		x[i] = (T)rand() / RAND_MAX;
		for (int j = 0; j < n; j++)
			matrix[i][j] = (T)rand() / RAND_MAX;
	}

	auto start_t = high_resolution_clock::now();
	mat_vec_mul(matrix, x, y, n, n);
	auto end_t = high_resolution_clock::now();

	cout << duration_cast<microseconds>(end_t - start_t).count();

	for (int i = 0; i < n; i++)
		delete[] matrix[i];
	delete[] matrix;
	return 0;
}