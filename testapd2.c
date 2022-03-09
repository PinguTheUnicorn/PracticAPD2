#include "mpi.h"
#include <stdio.h>
#include <stdlib.h>

int main (int argc, char *argv[])
{
	int  numtasks, rank;

	MPI_Init(&argc, &argv);
	MPI_Comm_size(MPI_COMM_WORLD, &numtasks);
	MPI_Comm_rank(MPI_COMM_WORLD,&rank);

	int recv_num;
	int initial_num = 2 * rank;
	MPI_Status status;

	/*
	int MPI_Send(void* data, int count, MPI_Datatype datatype, int destination, int tag, MPI_Comm communicator)
	int MPI_Recv(void* data, int count, MPI_Datatype datatype, int source, int tag, MPI_Comm communicator, MPI_Status* status)
	*/

	for (int i = 0; i < numtasks; i++) {
		if (i % 2 == 0) {
			if (rank % 2 == 0 && rank != numtasks - 1) {
				MPI_Send(&initial_num, 1, MPI_INT, rank + 1, 0, MPI_COMM_WORLD);
				MPI_Recv(&recv_num, 1, MPI_INT, rank + 1, 0, MPI_COMM_WORLD, &status);

				if (recv_num > initial_num) {
					initial_num = recv_num;
				}
			} else if (rank % 2 != 0) {
				MPI_Recv(&recv_num, 1, MPI_INT, rank - 1, 0, MPI_COMM_WORLD, &status);
				MPI_Send(&initial_num, 1, MPI_INT, rank - 1, 0, MPI_COMM_WORLD);

				if (recv_num < initial_num) {
					initial_num = recv_num;
				}
			}
		} else {
			if (rank % 2 != 0 && rank != numtasks - 1) {
				MPI_Send(&initial_num, 1, MPI_INT, rank + 1, 0, MPI_COMM_WORLD);
				MPI_Recv(&recv_num, 1, MPI_INT, rank + 1, 0, MPI_COMM_WORLD, &status);

				if (recv_num > initial_num) {
					initial_num = recv_num;
				}
			} else if (rank % 2 == 0 && rank != 0) {
				MPI_Recv(&recv_num, 1, MPI_INT, rank - 1, 0, MPI_COMM_WORLD, &status);
				MPI_Send(&initial_num, 1, MPI_INT, rank - 1, 0, MPI_COMM_WORLD);

				if (recv_num < initial_num) {
					initial_num = recv_num;
				}
			}
		}
	}

	printf("%d - %d\n", rank, initial_num);

	MPI_Barrier(MPI_COMM_WORLD);

	for (int i = 0; i < rank; i++) {
		MPI_Recv(&recv_num, 1, MPI_INT, rank - 1, 1, MPI_COMM_WORLD, &status);

		if (rank != numtasks - 1) {
			MPI_Send(&recv_num, 1, MPI_INT, rank + 1, 1, MPI_COMM_WORLD);
		} else {
			printf("%d ", recv_num);
		}
	}

	if (rank != numtasks - 1) {
		MPI_Send(&initial_num, 1, MPI_INT, rank + 1, 1, MPI_COMM_WORLD);
	} else {
		printf("%d\n", initial_num);
	}

	MPI_Finalize();

}
