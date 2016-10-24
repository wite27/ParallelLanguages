#include <stdio.h>
#include "mpi.h"
#define MAXPROC 128
#define NTIMES 10000
int main(int argc, char **argv)
{
   int rank, size, i, it;
   int ibuf[MAXPROC];
   double time_start, time_finish;
   MPI_Request req[2*MAXPROC];
   MPI_Status statuses[MAXPROC];
   MPI_Init(&argc, &argv);
   MPI_Comm_size(MPI_COMM_WORLD, &size);
   MPI_Comm_rank(MPI_COMM_WORLD, &rank);
   if(rank==0){
      for(i = 1; i<size; i++){
         MPI_Recv_init(&ibuf[i], 0, MPI_INT, i, 5, MPI_COMM_WORLD, &req[i]);
         MPI_Send_init(&rank, 0, MPI_INT, i, 6, MPI_COMM_WORLD, &req[size+i]);
      }
      time_start = MPI_Wtime();
      for(it = 0; it<NTIMES; it++){
         MPI_Startall(size-1, &req[1]);
         MPI_Waitall(size-1, &req[1], statuses);
         MPI_Startall(size-1, &req[size+1]);
         MPI_Waitall(size-1, &req[size+1], statuses);
      }
   }
   else{
      MPI_Recv_init(&ibuf[0], 0, MPI_INT, 0, 6, MPI_COMM_WORLD, &req[0]);
      MPI_Send_init(&rank, 0, MPI_INT, 0, 5, MPI_COMM_WORLD, &req[1]);
      time_start = MPI_Wtime();
      for(it = 0; it<NTIMES; it++){
         MPI_Start(&req[1]);
         MPI_Wait(&req[1], statuses);
         MPI_Start(&req[0]);
         MPI_Wait(&req[0], statuses);
      }
   }
   time_finish = MPI_Wtime()-time_start;
   printf("rank = %d model time = %lf\n", rank, time_finish/NTIMES);
   time_start = MPI_Wtime();
   for(it = 0; it<NTIMES; it++) MPI_Barrier(MPI_COMM_WORLD);
   time_finish = MPI_Wtime()-time_start;
   printf("rank = %d barrier time = %lf\n", rank, time_finish/NTIMES);
   MPI_Finalize();
}
