#include<iostream>

using namespace std;

template<typename T, typename F>
void sortiraj(T* niz, int n, F f) {
	for (int i = 0; i < n; i++) {
		for (int j = i + 1; j < n; j++) {
			if (f(niz[i], niz[j])) {
				T tmp = niz[i];
				niz[i] = niz[j];
				niz[j] = tmp;
			}
		}
	}
}

template<typename T>
struct poredjenje {
	bool operator()(T a, T b) {
		return a + 1 > b;
	}
};

int main() {

	int n = 10;
	int* a = new int[n];
	for (int i = 0; i < n; i++) a[i] = rand() % 50;
	poredjenje<int> p;
	sortiraj(a, n, p);
	for (int i = 0; i < n; i++) cout << a[i] << " ";

	delete[] a;

	return 0;
}