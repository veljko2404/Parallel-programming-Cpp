#include<iostream>

using namespace std;

int main() {

	int x = 10;
	auto kvadriranje = [](int x) { return x * x; };
	auto sabiranje = [](int a, int b) { return a + b; };
	cout << kvadriranje(x) << " " << sabiranje(x, x);

	return 0;
}