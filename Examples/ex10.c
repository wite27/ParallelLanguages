#include <stdio.h> 
#include "mpi.h"
#define NMAX 1000000
#define NTIMES 10
int main(int argc, char **argv)
{
   int rank, size, i, n, lmax;
   double time_start, time, bandwidth, max, a[NMAX];
   MPI_Status status;
   MPI_Init(&argc, &argv);
   MPI_Comm_size(MPI_COMM_WORLD, &size);
   MPI_Comm_rank(MPI_COMM_WORLD, &rank);
   time_start = MPI_Wtime();
   n = 0;
   max = 0.0;
   lmax = 0;
   while(n<=NMAX){
      time_start = MPI_Wtime();
      for(i = 0; i<NTIMES; i++){
         if(rank==0){
            MPI_Send(a, n, MPI_DOUBLE, 1, 1, MPI_COMM_WORLD);
            MPI_Recv(a, n, MPI_DOUBLE, 1, 1, MPI_COMM_WORLD, &status);
         }
         if(rank==1){
            MPI_Recv(a, n, MPI_DOUBLE, 0, 1, MPI_COMM_WORLD, &status);
            MPI_Send(a, n, MPI_DOUBLE, 0, 1, MPI_COMM_WORLD);
         }
      }
      time = (MPI_Wtime()-time_start)/(2*NTIMES);
      bandwidth = (sizeof(double)*n*1.0/(1024*1024*time));
      if(max<bandwidth){
         max = bandwidth;
         lmax = sizeof(double)*n;
      }
      if(rank==0)
         if(n==0) printf("latency = %lf seconds\n", time);
         else printf("%d bytes, bandwidth = %lf Mb/s\n", (int)sizeof(double)*n, bandwidth);
      if(n==0) n = 1;
      else n = 2*n;
   }
   if(rank==0) printf("max bandwidth = %lf Mb/s length = %d bytes\n", max, lmax);
   MPI_Finalize();
   return 0;}
