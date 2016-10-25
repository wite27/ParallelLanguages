#include <iostream>
#include <mpi.h>
#inclue <cmath>
#include <vector>

using namespace std;

void constructBasePrimes(vector<int> *basePrimes)
{
	int n = basePrimes[0];
	int baseCount = floor(sqrt(n))+1;
	
	bool isPrime[baseCount];
	isPrime[0] = false;
	isPrime[1] = true;
	if (2 > baseCount) return;
	
	for (int prime = 2; prime < baseCount && !basePrimes[prime]; prime++)
	{
		basePrimes.push_back(prime);
		for (int current = prime; current < baseCount; current *= prime)
		{
			isPrime[current] = true;
		}
	}
}

int main (int argc, **argv)
{
	int myrank, nproc, number, baseCount;
	int primesbuf[];
	vector<int> basePrimes;
	MPI_Init(&argc, &argv);
	MPI_Comm_rank(MPI_COMM_WORLD, &myrank);
	MPI_Comm_size(MPI_COMM_WORLD, &nproc);
	if (myrank == 0)
	{
		cout << "Number = ";
		cin >> number;
		starttime = MPI_Wtime();	
		basePrimes.push_back(number);
		constructBasePrimes(&basePrimes);
		primesbuf = new int[basePrimes.size()];
		copy(basePrimes.begin(), basePrimes.end(), primesbuf);
	}
	MPI_Bcast(&primesbuf, basePrimes.size(), MPI_INT, 0, MPI_COMM_WORLD);
	
}