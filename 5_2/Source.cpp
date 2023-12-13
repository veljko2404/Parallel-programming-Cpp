#include<iostream>
#include<omp.h>
#include<chrono>
using namespace std;
using namespace std::chrono;

typedef long long lng;

void v1() {
	lng num_steps = 1 << 20;
	double step = 1.0 / num_steps;
	double pi = 0;
	int num_threads = 8;
	auto start_t = high_resolution_clock::now();
#pragma omp parallel num_threads(num_threads) \
shared(num_steps,step,pi,num_threads)
	{
		int tid = omp_get_thread_num();
		int work_per_thread = (num_threads + num_steps - 1) / num_threads;
		int start = tid * work_per_thread;
		int end = tid == num_threads - 1 ? num_steps : (tid + 1) * work_per_thread;
		double partial_sum = 0;
		for (lng i = start; i < end; i++) {
			double x = (i + 0.5) * step;
			partial_sum += 4.0 / (1.0 + x * x);
		}
		partial_sum *= step;
	#pragma omp atomic
		pi += partial_sum;
	}
	auto end_t = high_resolution_clock::now();
	auto micro_secs = duration_cast<microseconds>(end_t - start_t).count();
	cout << micro_secs << ", " << pi << endl;
}

void v2() {
	lng num_steps = 1 << 20;
	double step = 1.0 / num_steps;
	double pi = 0;
	int num_threads = 8;
	auto start_t = high_resolution_clock::now();
#pragma omp parallel num_threads(num_threads) \
shared(num_steps,step,pi,num_threads)
	{
		int tid = omp_get_thread_num();
		double partial_sum = 0;
		for (lng i = tid; i < num_steps; i += num_threads) {
			double x = (i + 0.5) * step;
			partial_sum += 4.0 / (1.0 + x * x);
		}
		partial_sum *= step;
	#pragma omp atomic
		pi += partial_sum;
	}
	auto end_t = high_resolution_clock::now();
	cout << duration_cast<microseconds>(end_t - start_t).count() << ", " << pi << endl;;
}

void v3() {
	lng num_steps = 1 << 20;
	double step = 1.0 / num_steps;
	double pi = 0;
	auto start_t = high_resolution_clock::now();
#pragma omp parallel num_threads(8) \
shared(step,num_steps,pi)
	{
		double partial_sum = 0;
	#pragma omp for
		for (lng i = 0; i < num_steps; i++) {
			double x = (i + 0.5) * step;
			partial_sum += 4.0 / (1.0 + x * x);
		}
		partial_sum *= step;
	#pragma omp atomic
		pi += partial_sum;
	}
	auto end_t = high_resolution_clock::now();
	cout << duration_cast<microseconds>(end_t - start_t).count() << ", " << pi << endl;
}

void v4() {
	lng num_steps = 1 << 20;
	double step = 1.0 / num_steps;
	double pi = 0;
	int num_threads = 8;
	auto start_t = high_resolution_clock::now();
#pragma omp parallel num_threads(num_threads) \
shared(num_steps,step,pi)
	{
		double partial_sum = 0;
	#pragma omp for nowait
		for (lng i = 0; i < num_steps; i++) {
			double x = (i + 0.5) * step;
			partial_sum += 4.0 / (1.0 + x * x);
		}
		partial_sum *= step;
	#pragma omp atomic
		pi += partial_sum;
	}
	auto end_t = high_resolution_clock::now();
	cout << duration_cast<microseconds>(end_t - start_t).count() << ", " << pi << endl;
}

void v5() {
	lng num_steps = 1 << 20;
	double step = 1.0 / num_steps;
	double pi = 0;
	int num_threads = 8;
	auto start_t = high_resolution_clock::now();
#pragma omp parallel num_threads(num_threads) \
shared(num_steps,step,pi)
	{
	#pragma omp for reduction(+:pi)
		for (lng i = 0; i < num_steps; i++) {
			double x = (i + 0.5) * step;
			pi += 4.0 / (1.0 + x * x);
		}
	#pragma omp single
		pi *= step;
	}
	auto end_t = high_resolution_clock::now();
	cout << duration_cast<microseconds>(end_t - start_t).count() << ", " << pi << endl;
}

void v6() {
	lng num_steps = 1 << 20;
	double step = 1.0 / num_steps;
	double pi = 0;
	int num_threads = 8;
	auto start_t = high_resolution_clock::now();
		for (lng i = 0; i < num_steps; i++) {
			double x = (i + 0.5) * step;
			pi += 4.0 / (1.0 + x * x);
		}
		pi *= step;
	auto end_t = high_resolution_clock::now();
	cout << duration_cast<microseconds>(end_t - start_t).count() << ", " << pi << endl;
}

int main() {

	v1();
	v2();
	v3();
	v4();
	v5();
	v6();

	return 0;
}