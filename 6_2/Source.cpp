#include<iostream>
using namespace std;

struct Auto {
	string model;
	int year;
	Auto(string model, int year) :model(model), year(year) {}
	Auto() = default;
	friend ostream& operator<<(ostream& os, const Auto a) {
		os << "Model: " << a.model << ", Year: " << a.year << endl;
		return os;
	}
};

template<typename T, typename F>
void sortiraj(T* arr, int n, F compare) {
	for (int i = 0; i < n; i++) {
		for (int j = i + 1; j < n; j++) {
			if (compare(arr[i], arr[j])) {
				T tmp = arr[i];
				arr[i] = arr[j];
				arr[j] = tmp;
			}
		}
	}
}

struct Compare {
	bool operator()(Auto a, Auto b) {
		return a.year < b.year;
	}
};

int main() {

	int n = 100; Compare c;
	char abc[26] = {'a','b','c','d','e','f','g','h','i','j','k','l','m','n','o','p','q','r','s','t','u','v','w','x','y','z'};
	Auto* cars = new Auto[n];

	for (int i = 0; i < n; i++) {
		cars[i].year = rand() % 110 + 1910;
		for (int j = 0; j < 5; j++) {
			int k = rand() % 25;
			cars[i].model += abc[k];
		}
	}

	sortiraj<Auto>(cars, n, c);

	for (int i = 0; i < n; i++)
		cout << cars[i];

	delete[] cars;
	
	return 0;
}