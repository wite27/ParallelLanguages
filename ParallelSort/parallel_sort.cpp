#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>
#include <algorithm>

#define DEFAULT_SIZE 512

using namespace std;

bool comp(const int a, const int b)
{
    return a <= b;
}

void merge(int* low_a, int* low_b, int len)
{
	int* temp = new int[len*2];
	int i = 0, a = 0, b = 0;
	while (a < len && b < len)
	{
		if (*(low_a+a) <= *(low_b+b))
		{
			temp[i] = *(low_a+a);
			a++;
		} else
		{
			temp[i] = *(low_b+b);
			b++;
		}
		i++;
	}
	while (a < len)
	{
		temp[i++] = *(low_a+(a++));
	}
	while (b < len)
	{
		temp[i++] = *(low_b+(b++));
	}
	for (int i = 0; i < len*2; i++)
	{
		*(low_a+i) = temp[i];
	}
	delete[] temp;
}

int main(int argc, char **argv)
{	
	int myrank, nprocs, n, part_n;
	double stime, etime, time;
	int* array;
	
	MPI_Init(&argc, &argv);	
	MPI_Comm_rank(MPI_COMM_WORLD, &myrank);
	MPI_Comm_size(MPI_COMM_WORLD, &nprocs);
	MPI_Status status;
	
	if (argc >= 2)
	{
		n = atoi(argv[1]);
	} else
	{
		n = DEFAULT_SIZE;
	}
	array = new int[n];
	part_n = n / nprocs;
	
	if (myrank == 0)
	{
		for (int i = 0; i < n; i++)
		{
			array[i] = rand() % n;
		}
		stime = MPI_Wtime();
	}
	MPI_Bcast(array, n, MPI_INT, 0, MPI_COMM_WORLD);
	sort(array+myrank*part_n, 
			  array+(myrank+1)*part_n, &comp);
	int step = 2;
	while (nprocs != step / 2)
	{
		if (myrank % step == 0)
		{
			int sender = myrank + step / 2;
			int* begin_a = array+myrank*part_n;
			int* begin_b = array+(myrank+step/2)*part_n;
			int len = part_n * (step / 2);
			MPI_Recv(begin_b, len, MPI_INT, sender, 0, MPI_COMM_WORLD, &status);
			merge(begin_a, begin_b, len);
		} else if (myrank % (step / 2) == 0)
		{
			int receiver = myrank - step / 2;
			int* begin = array+myrank*part_n;
			int len = part_n * (step / 2);
			MPI_Send(begin, len, MPI_INT, receiver, 0, MPI_COMM_WORLD);
		}
		MPI_Barrier(MPI_COMM_WORLD);
		step *= 2;
	}
	if (myrank == 0)
	{		
		etime = MPI_Wtime();
		time = etime - stime;
		printf("Complited in %lf for %d elements on %d processors.\n", time, n, nprocs);
		/*for (int i = 0; i < n; i++)
			printf("a[%d]\t = %d, \n", i ,array[i]);
		printf("\n");*/
	}
	delete[] array;
	MPI_Finalize();
	return 0;
}