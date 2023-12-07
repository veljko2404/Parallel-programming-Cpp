#include<iostream>

using namespace std;

class kvadriraj {
public:
	int operator()(int x) {
		return x * x;
	}
};

class saberi {
public:
	int operator()(int a, int b){
		return a + b;
	}
};

class veca {
public:
	void operator()(int a, int b){
		if (a > b) {
			cout << "prvi veci";
		}
		else {
			cout << "prvi nije veci";
		}
	}
};

int main() {

	int x = 10;
	kvadriraj k; saberi s; veca v;
	cout << k(x) << endl << s(x, x) << endl;
	v(x, x);
	return 0;
}