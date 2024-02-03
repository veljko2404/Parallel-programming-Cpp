#include<iostream>
#include<mpi.h>
#include<vector>

using namespace std;

template<typename T>
T sum_array(T* array, int n) {
	T sum = 0;
	for (int i = 0; i < n; i++)
		sum += array[i];
	return sum;
}

int main(int argc, char** argv) {

	MPI_Init(&argc, &argv);

	int n = 1 << 20;
	int n_p ,i;
	int root_rank = 0;
	int curr_rank;
	MPI_Comm_rank(MPI_COMM_WORLD, &curr_rank);
	MPI_Comm_size(MPI_COMM_WORLD, &n_p);
	int elem_per_p = (n + n_p - 1) / n_p;
	vector<int> counts(n_p);
	vector<int> displacements(n_p);
	
	for (i = 0; i < n_p; i++) {
		counts[i] = elem_per_p;
		displacements[i] = elem_per_p * i;
	}
	counts[i] = n - (n_p - 1) * elem_per_p;
	displacements[i] = i * elem_per_p;
	int* local_array = new int[counts[curr_rank]];
	
	if (root_rank == curr_rank) {
		int* veliki_niz = new int[n];
		for (int i = 0; i < n; i++)
			veliki_niz[i] = 1;
		MPI_Scatterv(veliki_niz, counts.data(), displacements.data(), MPI_INT, local_array, counts[curr_rank], MPI_INT, root_rank, MPI_COMM_WORLD);
		delete[] veliki_niz;
	}
	else 
		MPI_Scatterv(0, 0, 0, MPI_INT, local_array, counts[curr_rank], MPI_INT, root_rank, MPI_COMM_WORLD);
	
	int local_result = sum_array(local_array, counts[curr_rank]);
	delete[] local_array;
	
	if (curr_rank == root_rank) {
		int* local_results = new int[n_p];
		MPI_Gather(&local_result, 1, MPI_INT, local_results, 1, MPI_INT, root_rank, MPI_COMM_WORLD);
		int final_result = sum_array(local_results, n_p);
		cout << final_result;
		delete[] local_results;
	}
	else
		MPI_Gather(&local_result, 1, MPI_INT, 0, 0, 0, root_rank, MPI_COMM_WORLD);

	MPI_Finalize();

	return 0;
}