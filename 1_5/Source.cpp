#include<iostream>
#include<algorithm>

using namespace std;

template<typename T, int size = 100>
class niz {
	T a[size];
public:
	T operator[](int i) const { return a[i]; }
	T& operator[](int i) { return a[i]; }
	class Iterator {
		T* ptr;
	public:
		Iterator(T* ptr) : ptr(ptr) {}
		T operator*() { return *ptr; }
		Iterator operator++(int) {
			Iterator tmp = *this;
			ptr++;
			return tmp;
		}
		Iterator& operator++() {
			ptr++;
			return *this;
		}
		bool operator==(const Iterator& drugi) {
			return drugi.ptr == ptr;
		}
		bool operator!=(const Iterator& drugi) {
			return !(drugi.ptr == ptr);
		}
	};
	Iterator begin() {
		return Iterator(a);
	}
	Iterator end() {
		return Iterator(a + size);
	}
};

int main() {

	niz<int, 5> a;
	a[0] = 4;
	a[1] = 8;
	a[2] = 3;
	a[3] = 1;
	a[4] = 6;
	for (int elem : a)
		cout << elem << " ";

	for_each(a.begin(), a.end(), [](int x) {
		cout << x * x << " ";
	});

	return 0;
}