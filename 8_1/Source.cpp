#include<iostream>
#include<omp.h>
#include<vector>
#include<chrono>
using namespace std;
using namespace std::chrono;

template<typename T>
struct Node {
	T x;
	Node<T>* l;
	Node<T>* r;
	Node(T x, Node<T>* l = nullptr, Node<T>* r = nullptr) : x(x), l(l), r(r) {}
	Node(const Node<T>& other) : x(other.x) {
		if (other.l) l = new Node(other.l);
		if (other.r) r = new Node(other.r);
	}
	~Node() {
		if (l) {
			delete l;
			l = nullptr;
		}
		if (r) {
			delete r;
			r = nullptr;
		}
	}
};

template<typename T>
void count_element_in_tree(Node<T>* head, int* result, int level = 0) {
	int num_l = 0, num_r = 0;
	if (!head) {
		*result = 0;
		return;
	}
#pragma omp parallel if(level == 0) shared(num_l, num_r) num_threads(8)
	{
#pragma omp parallel nowait
		{
#pragma omp task shared(num_l) if(level < 5)
			{
				count_element_in_tree(head->l, &num_l, level + 1);
			}
#pragma omp task shared(num_r) if(level < 5)
			{
				count_element_in_tree(head->r, &num_r, level + 1);
			}
#pragma omp barrier
#pragma omp single
			* result = 1 + num_l + num_r;
		}
	}
}


int main() {

	using T = int;
	int n = 1 << 10;
	vector<Node<T>*> curr_level(n);
	vector<Node<T>*> next_level(2 * n, nullptr);
	while (n > 0) {
		for (int i = 0; i < n; i++) {
			curr_level[i] = new Node<T>(i, next_level[i * 2], next_level[i * 2 + 1]);
		}
		n /= 2;
		next_level.clear();
		next_level = curr_level;
		curr_level.resize(n);
	}
	Node<T>* head = next_level[0];
	int num;
	auto start_t = high_resolution_clock::now();
	count_element_in_tree(head, &num);
	auto end_t = high_resolution_clock::now();
	cout << num << ", " << duration_cast<microseconds>(start_t - end_t).count();

	return 0;
}