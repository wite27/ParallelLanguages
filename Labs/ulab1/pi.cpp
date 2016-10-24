#include <iostream>
#include <mpi.h>

using namespace std;

double f(double x)
{
	return 1/(1+x*x);
}
int main (int argc, char **argv)
{
	double starttime, endtime, time;
	double pi, sum = 0, term, h;
	int myrank, nproc, n, i;
	MPI_Init(&argc, &argv);
	MPI_Comm_rank(MPI_COMM_WORLD, &myrank);
	MPI_Comm_size(MPI_COMM_WORLD, &nproc);
	if (myrank == 0)
	{
		cout << "Number of iterations=";
		cin >> n;
		starttime = MPI_Wtime();
	}
	MPI_Bcast(&n, 1, MPI_INT, 0, MPI_COMM_WORLD);
	h = 1.0/n;
	for (i = myrank+1; i <= n; i += nproc)
	{
		sum += f(h*(i-0.5));
	}
	term = 4*h*sum;
	MPI_Reduce(&term, &pi, 1, MPI_DOUBLE, MPI_SUM, 0, MPI_COMM_WORLD);
	if (myrank == 0)
	{
		endtime = MPI_Wtime();
		time = endtime - starttime;
		cout << "Computed value of pi = " << pi << endl;
		cout << "Iterations = " << n << ", Number of processes = " << nproc << endl;
		cout << "Elapsed time = " << time << endl;
	}
	MPI_Finalize();
	return 0;
}