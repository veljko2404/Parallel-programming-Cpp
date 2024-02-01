#include<iostream>
#include<chrono>

using namespace std;
using namespace std::chrono;

template<typename T>
class Matrix {
	T* ptr_to_data = nullptr;
public:
	int n, m;
	Matrix(int n, int m, bool rand_init = false) :n(n), m(m) {
		allocate_memory(rand_init);
	}
	Matrix(const Matrix& other) :n(other.n), m(other.m) {
		ptr_to_data = new T[n * m];
		if (other.ptr_to_data)
			memcpy(ptr_to_data, other.ptr_to_data, n * m * sizeof(T));
	}
	Matrix& operator=(const Matrix& other) {
		if (this == *other)return *this;
		if (n * m != other.m * other.n) {
			delete[] ptr_to_data;
			n = other.n;
			m = other.m;
			allocate_memory(false);
		}
		if (other.ptr_to_data)
			memcpy(ptr_to_data, other.ptr_to_data, n * m * sizeof(T));
		return *this;
	}
	Matrix(Matrix&& other) :n(other.n), m(other.m), ptr_to_data(other.ptr_to_data) {
		other.ptr_to_data = nullptr;
	}
	Matrix& operator=(Matrix&& other) {
		if (this == &other)return *this;
		delete[] ptr_to_data;
		n = other.n;
		m = other.m;
		ptr_to_data = other.ptr_to_data;
		other.ptr_to_data = nullptr;
		return *this;
	}
	inline const T operator()(int i, int j)const { return ptr_to_data[i * m + j]; }
	inline T& operator()(int i, int j) { return ptr_to_data[i * m + j]; }
	Matrix get_submatrix(int i_start, int i_end, int j_start, int j_end) const {
		int submatrix_n = i_end - i_start;
		int submatrix_m = j_end - j_start;
		Matrix<T> tmp(submatrix_n, submatrix_m);
#pragma omp parallel for default(shared)
		for (int i = 0; i < submatrix_n; i++) {
			for (int j = 0; j < submatrix_m; j++)
				tmp(i, j) = this->operator()(i + i_start, j + j_start);
		}
		return std::move(tmp);
	}
	void set_submatrix(const Matrix& SM, int i_start, int j_start) {
		for (int i = 0; i < SM.n; i++) {
			for (int j = 0; j < SM.m; j++)
				this->operator()(i + i_start, j + j_start) = SM(i, j);
		}
	}
	void print() {
		for (int i = 0; i < n; i++) {
			for (int j = 0; j < m; j++)
				cout << this->operator()(i, j) << " ";
			cout << endl;
		}
	}
private:
	void allocate_memory(bool rand_init) {
		if (ptr_to_data)return;
		ptr_to_data = new T[n * m];
		if (rand_init) {
#pragma omp parallel for
			for (int i = 0; i < n * m; i++)
				ptr_to_data[i] = (T)rand() % 10;
		}
	}
};

template<typename T>
Matrix<T> operator+(const Matrix<T>& A, const Matrix<T> B) {
	Matrix<T> C(A.n, A.m);
	for (int i = 0; i < A.n; i++)
		for (int j = 0; j < A.m; j++)
			C(i, j) = A(i, j) + B(i, j);
	return std::move(C);
}

template<typename T>
Matrix<T> operator-(const Matrix<T>& A, const Matrix<T> B) {
	Matrix<T> C(A.n, A.m);
	for (int i = 0; i < A.n; i++)
		for (int j = 0; j < A.m; j++)
			C(i, j) = A(i, j) - B(i, j);
	return std::move(C);
}

template<typename T>
Matrix<T> matrix_multiplication(const Matrix<T>& A, const Matrix<T>& B, bool use_omp = true) {
	Matrix<T> C(A.n, B.m);
#pragma omp parallel for default(shared) if(use_omp)
	for (int i = 0; i < C.n; i++) {
		for (int j = 0; j < C.m; j++) {
			T cij = 0;
			for (int k = 0; k < A.m; k++)
				cij += A(i, k) * B(k, j);
			C(i, j) = cij;
		}
	}
	return std::move(C);
}

template<typename T>
Matrix<T> matrix_multiplication_strassen(const Matrix<T>& X, const Matrix<T>& Y) {
	int n = X.n;
	Matrix<T> Z(n, n);
	Matrix<T> B = X.get_submatrix(0, n / 2, 0, n / 2);
	Matrix<T> A = X.get_submatrix(0, n / 2, n / 2, n);
	Matrix<T> C = X.get_submatrix(n / 2, n, 0, n / 2);
	Matrix<T> D = X.get_submatrix(n / 2, n, n / 2, n);
	Matrix<T> E = X.get_submatrix(0, n / 2, 0, n / 2);
	Matrix<T> F = X.get_submatrix(0, n / 2, n / 2, n);
	Matrix<T> G = X.get_submatrix(n / 2, n, 0, n / 2);
	Matrix<T> H = X.get_submatrix(n / 2, n, n / 2, n);

	Matrix<T> I(n / 2, n / 2), J(n / 2, n / 2), K(n / 2, n / 2), L(n / 2, n / 2);
	Matrix<T> M1(n / 2, n / 2), M2(n / 2, n / 2), M3(n / 2, n / 2), M4(n / 2, n / 2), M5(n / 2, n / 2), M6(n / 2, n / 2), M7(n / 2, n / 2);
	bool parallel_matrix_multiplication = false;
#pragma omp parallel default(shared) firstprivate(parallel_matrix_multiplication) num_threads(8)
#pragma omp single
	{
#pragma omp task depend(in: A, C, E, F) depend(out: M1)
		{ M1 = matrix_multiplication(A + C, E + F, parallel_matrix_multiplication); }
#pragma omp task depend(in: B, D, G, H) depend(out: M2)
		{ M2 = matrix_multiplication(B + D, G + H, parallel_matrix_multiplication); }
#pragma omp task depend(in: A, D, E, H) depend(out: M3)
		{ M3 = matrix_multiplication(A - D, E + H, parallel_matrix_multiplication); }
#pragma omp task depend(in: A, F, H) depend(out: M4)
		{ M4 = matrix_multiplication(A, F - H, parallel_matrix_multiplication); }
#pragma omp task depend(in: C, D, E) depend(out: M5)
		{ M5 = matrix_multiplication(C + D, E, parallel_matrix_multiplication); }
#pragma omp task depend(in: A, B, H) depend(out: M6)
		{ M6 = matrix_multiplication(A + B, H, parallel_matrix_multiplication); }
#pragma omp task depend(in: D, G, E) depend(out: M7)
		{ M7 = matrix_multiplication(D, G - E, parallel_matrix_multiplication); }

#pragma omp task depend(in: M2, M3, M6, M7) depend(out: I)
		{ I = M2 + M3 - M6 - M7; }
#pragma omp task depend(in: M4, M6) depend(out: J)
		{ J = M4 + M6; }
#pragma omp task depend(in: M5, M7) depend(out: K)
		{ K = M5 + M7; }
#pragma omp task depend(in: M1, M3, M4, M5) depend(out: L)
		{ L = M1 - M3 - M4 - M5; }

#pragma omp task depend(in: I)
		{ Z.set_submatrix(I, 0, 0); }
#pragma omp task depend(in: J)
		{ Z.set_submatrix(J, 0, n / 2); }
#pragma omp task depend(in: K)
		{ Z.set_submatrix(K, n / 2, 0); }
#pragma omp task depend(in: L)
		{ Z.set_submatrix(L, n / 2, n / 2); }
	}
	return Z;
}

template<typename T>
bool mat_equal(const Matrix<T>& A, const Matrix<T>& B, T eps = 1e-6) {
	if (A.n != B.n || A.m != B.m) return false;
	for (int i = 0; i < A.n; i++)
		for (int j = 0; j < A.m; j++)
			if (abs(A(i, j) - B(i, j)) < eps) return false;
	return true;
}

int main() {

	using T = int;
	int n = 1 << 4;
	Matrix<T> A(n, n, true), B(n, n, true);
	auto start_t = high_resolution_clock::now();
	Matrix<T> C = matrix_multiplication(A, B);
	Matrix<T> D = matrix_multiplication_strassen(A, B);
	auto end_t = high_resolution_clock::now();
	cout << "Time for matrix multiplications: " << duration_cast<microseconds>(end_t - start_t).count() << endl;
	cout << "Matrix A:" << endl;
	A.print(); cout << endl;
	cout << "Matrix B:" << endl;
	B.print(); cout << endl;
	cout << "Matrix A x B" << endl;
	C.print(); cout << endl;
	cout << "Matrix A x B using strassen algorithm:" << endl;
	D.print();
	return 0;
}