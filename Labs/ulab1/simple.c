#include <iostream>
#include <mpi.h>

using namespace std;

inc main (int argc, **argv)
{
	int myrank, nproc;
	MPI_Init(&argc, &argv);
	MPI_Comm_rank(MPI_COMM_WORLD, &myrank);
	MPI_Comm_size(MPI_COMM_WORLD, &nproc);
	if (myrank == 0)
	{
		
	}
	
}