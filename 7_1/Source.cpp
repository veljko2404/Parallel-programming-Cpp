#include<iostream>
#include<omp.h>
using namespace std;

template<typename T>
int br_elem_razlicitih_od_0(T* niz, int n, T eps=1e-6) {
	int count = 0;
#pragma omp parallel for reduction(+:count)
	for (int i = 0; i < n; i++)
		count += (abs(niz[i] > eps));
	return count;
}

int main() {

	using T = double;
	T niz[] = { 1,4,0.00000006,4,-0.000000004,1 };
	int n = 7;
	cout << br_elem_razlicitih_od_0(niz, n);

	return 0;
}