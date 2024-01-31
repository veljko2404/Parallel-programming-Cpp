#include<iostream>
#include<omp.h>
#include<cmath>
#include<chrono>
using namespace std;
using namespace std::chrono;

template<typename T>
void br_elem_razlicitih_od_0(T* niz, int from, int to, int& non_zero, T eps = 1e-6) {
	int count = 0;
	for (int i = from; i < to; i++) {
		count += (abs(niz[i]) > eps);
	}
	#pragma omp atomic
	non_zero += count;
}

template<typename T>
bool is_upper_triangular(T** matrix, int n, T eps = 1e-6, int num_threads = 8) {
	int non_zero = 0;
	#pragma omp parallel num_threads(num_threads) shared(non_zero,eps,n,matrix)
	{
		#pragma omp single nowait
		{
			for (int i = 1; i < n; i++) {
				#pragma omp task
				br_elem_razlicitih_od_0(matrix[i], 0, i, non_zero, eps);
			}
		}
	}
	return non_zero == 0;
}

template<typename T>
void transform_row(T** A, T* b, int n, int row_i, int row_j, T factor) {
	for (int j = row_i; j < n; j++)
		A[row_j][j] += A[row_i][j] * factor;
	b[row_j] += b[row_i] * factor;
}

template<typename T>
void gauss_elimination(T** matrix, T* b, T* x, int n, int num_threads = 8) {
	int pivot_i = 0;
	#pragma omp parallel num_threads(num_threads) shared(pivot_i)
	{
		while (pivot_i < n) {
		#pragma omp single
		{
			int pivot_j = pivot_i;
			T pivot = matrix[pivot_i][pivot_j];
			for (int i = pivot_i + 1; i < n; i++) {
				T factor = -1 * matrix[i][pivot_i] / pivot;
				#pragma omp task if(n-pivot_i > 256)
				transform_row(matrix, b, n, pivot_i, i, factor);
			}
		}
		#pragma omp single
		pivot_i++;
		}
	}
	for (int i = n - 1; i >= 0; i--) {
		x[i] = b[i] / matrix[i][i];
#pragma omp parallel for
		for (int row_above = i - 1; row_above >= 0; row_above--)
			b[row_above] -= x[i] * matrix[row_above][i];
	}
}

int main() {

	using T = double;
	int n = 1 << 10;
	T** matrix = new T*[n];
	T* b = new T[n];
	T* x = new T[n];
	for (int i = 0; i < n; i++) {
		matrix[i] = new T[n];
	}

	for (int i = 0; i < n; i++) {
		b[i] = (T)rand() / RAND_MAX;
		for (int j = 0; j < n; j++)
			matrix[i][j] = (T)rand() / RAND_MAX;
	}

	auto start_t = high_resolution_clock::now();
	gauss_elimination(matrix, b, x, n, 16);
	auto end_t = high_resolution_clock::now();
	cout << duration_cast<microseconds>(end_t - start_t).count() << endl;

	cout << is_upper_triangular(matrix, n) << endl;


	for (int i = 0; i < n; i++) {
		delete[] matrix[i];
	}
	delete[] matrix;
	delete[] b;
	delete[] x;

	return 0;
}
