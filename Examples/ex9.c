#include <stdio.h>
#include "mpi.h"
int main(int argc, char **argv)
{ int rank, size, ibuf;
   MPI_Status status;
   float rbuf;
   MPI_Init(&argc, &argv);
   MPI_Comm_size(MPI_COMM_WORLD, &size);
   MPI_Comm_rank(MPI_COMM_WORLD, &rank);
   ibuf = rank;
   rbuf = 1.0 * rank;
   if(rank==1) MPI_Send(&ibuf, 1, MPI_INT, 0, 5, MPI_COMM_WORLD);
   if(rank==2) MPI_Send(&rbuf, 1, MPI_FLOAT, 0, 5, MPI_COMM_WORLD);
   if(rank==0){
      MPI_Probe(MPI_ANY_SOURCE, 5, MPI_COMM_WORLD, &status);
      if(status.MPI_SOURCE == 1){
         MPI_Recv(&ibuf, 1, MPI_INT, 1, 5, MPI_COMM_WORLD, &status);
         MPI_Recv(&rbuf, 1, MPI_FLOAT, 2, 5, MPI_COMM_WORLD, &status);
      }
      else if(status.MPI_SOURCE == 2){
         MPI_Recv(&rbuf, 1, MPI_FLOAT, 2, 5, MPI_COMM_WORLD, &status);
         MPI_Recv(&ibuf, 1, MPI_INT, 1, 5, MPI_COMM_WORLD, &status);
      }
      printf("Process 0 recv %d from process 1, %f from process 2\n", ibuf, rbuf);
   }
   MPI_Finalize();
   return 0;
}
