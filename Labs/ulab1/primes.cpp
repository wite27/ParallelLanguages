#include <iostream>
#include <mpi.h>
#include <cmath>
#include <vector>
#define MAXN 10000

using namespace std;

void constructBasePrimes(vector<int> *basePrimes)
{
	int n = (*basePrimes)[0];
	int baseCount = floor(sqrt(n))+1;
	
	bool isPrime[baseCount];
	for (int i = 0; i < baseCount; i++)
	{
		isPrime[i] = false;
	}
	isPrime[0] = false;
	isPrime[1] = true;
	if (2 > baseCount) return;
	
	for (int prime = 2; prime < baseCount; prime++)
	{
		if (!isPrime[prime])
		{
			(*basePrimes).push_back(prime);
			for (int current = prime; current < baseCount; current += prime)
			{
				isPrime[current] = true;
			}
		}
	}
}

bool isPrime(int n, int* basePrimes, int baseCount)
{
	for (int i = 1; i < baseCount; i++)
	{
		if (n % (basePrimes[i]) == 0)
		{
			return false;
		}
	}
	return true;
}
int main (int argc, char **argv)
{
	int myrank, nproc, number, baseCount, partCount = 0, result;
	double starttime, endtime, time;
	int primesbuf[MAXN];
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
		baseCount = basePrimes.size();
		copy(basePrimes.begin(), basePrimes.end(), primesbuf);
	}
	MPI_Bcast(&baseCount, 1, MPI_INT, 0, MPI_COMM_WORLD);
	MPI_Bcast(&primesbuf, MAXN, MPI_INT, 0, MPI_COMM_WORLD);
	number = primesbuf[0];
	//if (myrank == 0)
	/*{
		cout << "Proc " << myrank << ": 1 ";
		for (int i = 1; i < baseCount; i++)
		{
			cout << primesbuf[i] << " ";
		}
		cout << endl;
	}*/
	if (myrank == 0)
	{
		partCount = baseCount - 1;
	}
	for (int i = baseCount+myrank; i < number; i += nproc)
	{
		//cout << "Processing " << i << endl;
		if (isPrime(i, primesbuf, baseCount))
		{
			//cout << i << " is prime" << endl;
			partCount++;
		}
	}
	MPI_Reduce(&partCount, &result, 1, MPI_INT, MPI_SUM, 0, MPI_COMM_WORLD);
	if (myrank == 0)
	{
		endtime = MPI_Wtime();
		time = endtime - starttime;
		cout << "Count of prime nubmers in {1.." << number << "} = " << result << endl;
		cout << "Elapsed time = " << time << endl;
	}
	MPI_Finalize();
	return 0;
}