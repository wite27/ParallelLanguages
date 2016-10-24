#include <stdio.h>
#include "mpi.h"
#define N 8
void work(int n, int nl, int size, int rank)
{
   int ii, matr_rev;
   double a[n][nl], b[n][nl];
   int i, j;
   MPI_Status status;
   for(j = 0; j<nl; j++)
      for(i = 0; i<n; i++){
         b[i][j] = 0.0;
         ii = j+rank*nl;
         a[i][j] = 100*ii+i;
      }
   MPI_Type_vector(nl, n, -n, MPI_DOUBLE, &matr_rev);
   MPI_Type_commit(&matr_rev);
   MPI_Sendrecv(&a[0][nl-1], 1, matr_rev, size-rank-1, 1, b, nl*n, MPI_DOUBLE, size-rank-1, 1, MPI_COMM_WORLD, &status);
   for(j = 0; j<nl; j++)
   for(i = 0; i<n; i++)
   printf("process %d : %d %d %f %f\n", rank, j+rank*nl, i, a[i][j], b[i][j]);
}
int main(int argc, char **argv)
{
   int rank, size,nl;
   MPI_Init(&argc, &argv);
   MPI_Comm_size(MPI_COMM_WORLD, &size);
   MPI_Comm_rank(MPI_COMM_WORLD, &rank);
   nl = (N-1)/size+1;
   work(N, nl, size, rank);
   MPI_Finalize();
}
