#include <stdio.h>
#include "mpi.h"
#define n 1000
int main(int argc, char **argv)
{
   int rank, i, size, nproc;
   double time_start, time_finish;
   double a[n], b[n], c[n];
   MPI_Status status;
   MPI_Init(&argc, &argv);
   MPI_Comm_size(MPI_COMM_WORLD, &size);
   MPI_Comm_rank(MPI_COMM_WORLD, &rank);
   nproc = size;
   for(i = 0; i<n; i++) a[i] = 1.0/size;
   MPI_Barrier(MPI_COMM_WORLD);
   time_start = MPI_Wtime();
   for(i = 0; i<n; i++) c[i] = a[i];
   while(nproc>1){
      if(rank<nproc/2){
         MPI_Recv(b, n, MPI_DOUBLE, nproc-rank-1, 1, MPI_COMM_WORLD, &status);
         for(i = 0; i<n; i++) c[i] = c[i] + b[i];
      }
      else if(rank<nproc)
         MPI_Send(c, n, MPI_DOUBLE, nproc-rank-1, 1, MPI_COMM_WORLD);
      nproc = nproc/2;
   }
   for(i = 0; i<n; i++) b[i] = c[i];
   time_finish = MPI_Wtime()-time_start;
   if(rank==0) printf("model b[1]=%lf\n", b[1]);
   printf ("rank=%d model time=%lf\n", rank, time_finish);
   for(i = 0; i<n; i++) a[i] = 1.0/size;
   MPI_Barrier(MPI_COMM_WORLD);
   time_start = MPI_Wtime();
   MPI_Reduce(a, b, n, MPI_DOUBLE, MPI_SUM, 0, MPI_COMM_WORLD);
   time_finish = MPI_Wtime()-time_start;
   if(rank==0) printf("reduce b[1]=%lf\n", b[1]);
   printf("rank=%d reduce time =%lf\n", rank, time_finish);
   MPI_Finalize();
}
