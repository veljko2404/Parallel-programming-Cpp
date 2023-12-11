#include<iostream>
#include<chrono>
#include<thread>

using namespace std;

template<typename T>
void quick_sort(T* arr, int l, int r) {
	if (l > r) return;
	T pivot = arr[r], j = l, i = l;
	while (i <= r) {
		if (arr[i] <= pivot) {
			T tmp = arr[i];
			arr[i] = arr[j];
			arr[j] = tmp;
			j++; i++;
		}
		else {
			i++;
		}
	}
	j--;
	int left_part = j - l;
	int right_part = r - j;
	if (left_part > right_part && left_part > 5000) {
		thread t(quick_sort<T>, arr, l, j - 1);
		quick_sort(arr, j + 1, r);
		t.join();
	}
	else if (right_part > 5000) {
		thread t(quick_sort<T>, arr, j + 1, r);
		quick_sort(arr, l, j - 1);
		t.join();
	}
	else {
		quick_sort(arr, l, j - 1);
		quick_sort(arr, j + 1, r);
	}
}

int main() {

	int n = 50000;
	int* a = new int[n];
	for (int i = 0; i < n; i++) a[i] = rand() % 100;
	auto time_start = chrono::high_resolution_clock::now();
	quick_sort(a, 0, n - 1);
	auto time_end = chrono::high_resolution_clock::now();
	cout << chrono::duration_cast<chrono::microseconds>(time_end - time_start).count();
	delete[] a;
	return 0;
}