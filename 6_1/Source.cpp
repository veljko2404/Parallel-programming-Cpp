#include<vector>
#include<iostream>
#include<omp.h>

using namespace std;

template<typename T>
class vektor {
	vector<T> v;
public:
	using type_name = T;
	vektor() = default;
	vektor(int n) :v(vector<T>(n)) {}
	T& operator[](int i) { return v[i]; }
};

struct plus {
	template<typename T>
	T operator()(const T& a, const T& b) const {
		return a + b;
	}
};

struct minus {
	template<typename T>
	T operator()(const T& a, const T& b) const {
		return a - b;
	}
};

struct puta {
	template<typename T>
	T operator()(const T& a, const T& b) {
		return a * b;
	}
};

template<typename BinOp, typename L, typename R>
class binarnaOperacija {
public:
	using value_type = typename L::value_type;
private:
	BinOp binOp;
	const L& leviOperand;
	const R& desniOperand;
public:
	binarnaOperacija(BinOp binOp, const L& levi, const R& desni) :binOp(binOp), leviOperand(leviOperand), desniOperand(desniOperand) {}
	value_type operator[](int i)const {
		return binOp(leviOperand[i], desniOperand[i]);
	}
	int size() const { return leviOperand.size(); }
};

template<typename L, typename R>
auto operator+ (const L& l, const R& r) {
	return binarnaOperacija(plus(), l, r);
}

int main() {

	int n = 10;
	vektor<int> a(n), b(n);
	for (int i = 0; i < n; i++) {
		a[i] = rand() % 100;
		b[i] = rand() % 100;
	}

	auto c = a + b;

	vektor<int> rezultat = c;

	for (int i = 0; i < n; i++)
		cout << rezultat[i] << " " << endl;

	return 0;
}