#include <mpi.h>
#include <stdio.h>

#define n 1000

int main(int argc, char **argv)
{
	int myrank, nprocs, i, j, k, map[n];
	double stime, etime, time, a[n][n];
	FILE *f;
	
	MPI_Init(&argc, &argv);
	MPI_Comm_rank(MPI_COMM_WORLD, &myrank);
	MPI_Comm_size(MPI_COMM_WORLD, &nprocs);
	
	if (myrank == 0)
	{
		for (i = 0; i < n; i++)
		{
			for (j = 0; j < n; j++)
			{
				a[i][j] = 1.0 / (i+j+1);
			}
		}
		stime = MPI_Wtime();
	}
	
	MPI_Bcast(a, n*n, MPI_DOUBLE, 0, MPI_COMM_WORLD);
	
	for (i = 0; i < n; i++)
	{
		map[i] = i % nprocs;
	}
	
	for (k = 0; k<n-1;k++)
	{
		if (map[k] == myrank)
			for ( i = k+1; i<n; i++)
				a[k][i] /= a[k][k];
			MPI_Bcast(&a[k][k+1], n-k-1,MPI_DOUBLE, map[k], MPI_COMM_WORLD);
			for ( i = k+1; i<n; i++)
				if (map[i] == myrank)
					for ( j = k+1; j< n; j++)
						a[i][j] -= a[i][k]*a[k][j];
	}
	
	MPI_Barrier(MPI_COMM_WORLD);
	
	if (myrank == 0)
	{
		etime = MPI_Wtime();
		time = etime - stime;
		printf("Complited in %lf\n", time);
	}
	
	for( i = 0; i< n; i++)
		if (map[i] == myrank)
			{
				for (j=0; j<n;j++)
				printf("a[%d][%d] = %lg, \n", i,j,a[i][j]);
				printf("\n");
			}
	
	
	MPI_Finalize();
	return 0;
}