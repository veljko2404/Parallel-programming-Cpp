#include<iostream>
#include<mpi.h>

using namespace std;

template<typename T>
T sum_array(T* arr, int n) {
	T sum = 0;
	for (int i = 0; i < n; i++)
		sum += arr[i];
	return sum;
}

int main(int argc, char** argv) {

	int root_rank = 0;
	int curr_rank, p_num;
	constexpr int elem_per_process = 1000;
	int local_arr[elem_per_process];
	MPI_Init(&argc, &argv);

	MPI_Comm_size(MPI_COMM_WORLD, &p_num);
	MPI_Comm_rank(MPI_COMM_WORLD, &curr_rank);

	if (curr_rank == root_rank) {
		int* arr = new int[p_num * elem_per_process];
		for (int i = 0; i < p_num * elem_per_process; i++)
			arr[i] = rand() % 2;

		MPI_Scatter(arr, elem_per_process, MPI_INT, local_arr, elem_per_process, MPI_INT, root_rank, MPI_COMM_WORLD);
		delete[] arr;
	}
	else {
		MPI_Scatter(0, 0, 0, local_arr, elem_per_process, MPI_INT, root_rank, MPI_COMM_WORLD);
	}

	int local_result = sum_array(local_arr, elem_per_process);
	if (curr_rank == root_rank) {
		int* sums = new int[p_num];
		MPI_Gather(&local_result, 1, MPI_INT, sums, 1, MPI_INT, root_rank, MPI_COMM_WORLD);
		int result = sum_array(sums, p_num);
		delete[] sums;
		cout << result;
	}

	MPI_Gather(&local_result, 1, MPI_INT, 0, 0, 0, root_rank, MPI_COMM_WORLD);

	MPI_Finalize();

	return 0;
}