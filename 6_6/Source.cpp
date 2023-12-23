#include<iostream>
#include<omp.h>
#include<vector>
#include<chrono>
using namespace std;

int zbr(int* a, int n) {
	int res = 0;
#pragma omp parallel for num_threads(8) shared(res)
	for (int i = 0; i < n; i++) {
	#pragma omp atomic
		res += a[i];
	}
	return res;
}

int zbr_normal(int* a, int n) {
	int res = 0;
	for (int i = 0; i < n; i++) res += a[i];
	return res;
}

int main() {

	int n = 10000000;
	int* a = new int[n];

	for (int i = 0; i < n; i++) a[i] = rand() % 100;

	auto start1 = chrono::high_resolution_clock::now();
	int result1 = zbr(a, n);
	auto end1 = chrono::high_resolution_clock::now();

	auto start2 = chrono::high_resolution_clock::now();
	int result2 = zbr_normal(a, n);
	auto end2 = chrono::high_resolution_clock::now();
	
	cout << result1 << " in " << chrono::duration_cast<chrono::microseconds>(end1 - start1).count() << endl;
	cout << result2 << " in " << chrono::duration_cast<chrono::microseconds>(end2 - start2).count() << endl;

	delete[] a;
	return 0;
}