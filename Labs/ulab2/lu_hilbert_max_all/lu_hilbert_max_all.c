#include <mpi.h>
#include <stdio.h>
#include <float.h>

#define n 1000

void swap(int i1, int j1, int i2, int j2, double array[][n])
{
	double temp = array[i1][j1];
	array[i1][j1] = array[i2][j2];
	array[i2][j2] = temp;
}

struct { 
    double value; 
    int index; 
} in, out;

int main(int argc, char **argv)
{	
	const double lowest_double = -DBL_MAX;
	int myrank, nprocs, i, j, k, maxi, maxj, map[n], outi, outj;
	double stime, etime, time, a[n][n], max, buf[n];
	MPI_Status status;
	
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
		max = lowest_double;
		maxi = -1;
		maxj = -1;
		for (i = k+1; i<n && (map[i] == myrank); i++)
		{
			for (j = k; j<n; j++)
			{
				if (a[i][j] > max)
				{
					max = a[i][j];
					maxi = i;
					maxj = j;
				}
			}
		}
		in.value = max;
		in.index = maxi*n+maxj;
		
		MPI_Reduce(&in, &out, 1, MPI_DOUBLE_INT, MPI_MAXLOC, map[k], MPI_COMM_WORLD);
		MPI_Bcast(&out, 1, MPI_DOUBLE_INT, map[k], MPI_COMM_WORLD);
		outi = out.index / n;
		outj = out.index % n;
		if (map[k] == myrank)
		{
			if (max == out.value)
			{
				swap(k, k, k, outj, a);
				for (i = k+1; i < n; i++)
					a[k][i] /= a[k][k];
			} else
			{
				MPI_Recv(&buf[k], n-k, MPI_DOUBLE, map[outi], 1, MPI_COMM_WORLD, &status);
				MPI_Send(&a[k][k], n-k, MPI_DOUBLE, map[outi], 0, MPI_COMM_WORLD);
				for (i = k; i < n; i++)
				{
					a[k][i] = buf[i];
				}
				swap(k, k, k, outj, a);
				for (i = k+1; i < n; i++)
					a[k][i] /= a[k][k];
			}
		} else if (map[outi] == myrank)
		{
			MPI_Send(&a[k][k], n-k, MPI_DOUBLE, map[k], 1, MPI_COMM_WORLD);
			MPI_Recv(&buf[k], n-k, MPI_DOUBLE, map[k], 0, MPI_COMM_WORLD, &status);
			for (i = k; i < n; i++)
				a[k][i] = buf[i];
		}
		
		MPI_Bcast(&a[k][k+1], n-k-1,MPI_DOUBLE, map[k], MPI_COMM_WORLD);
		for (i = k+1; i<n; i++)
		{
			if (map[i] == myrank)
			{
				swap(i, k, i, outj, a);
				for (j = k+1; j< n; j++)
				{
					a[i][j] -= a[i][k]*a[k][j];
				}
			}
		}
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