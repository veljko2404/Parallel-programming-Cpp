#include<iostream>
#include<mpi.h>

using namespace std;

int main(int argc, char** argv) {

	MPI_Init(&argc, &argv);

	int curr_rank, root_rank = 0, local_value;

	MPI_Comm_rank(MPI_COMM_WORLD, &curr_rank);

	if (curr_rank == root_rank) {
		int arr[] = { 1,2,3,4 };
		MPI_Scatter(arr, 1, MPI_INT, &local_value, 1, MPI_INT, root_rank, MPI_COMM_WORLD);
	}
	else {
		MPI_Scatter(0, 0, 0, &local_value, 1, MPI_INT, root_rank, MPI_COMM_WORLD);
		cout << "Process " << curr_rank << ", value: " << local_value;
	}

	MPI_Finalize();

	return 0;
}
