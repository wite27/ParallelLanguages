#include <stdio.h>
#include "mpi.h"
void master(MPI_Comm comm)
{
   int size, i, r;
   MPI_Status status;
   MPI_Comm_remote_size(comm, &size);
   for(i=0; i<size; i++){
      MPI_Send(&i, 1, MPI_INT, i, 5, comm);
   }
   MPI_Recv(&r, 1, MPI_INT, 0, 6, comm, &status);
   printf("sum = %d\n", r);
}
void slave(MPI_Comm comm, MPI_Comm intercomm)
{
   int s, r, rank;
   MPI_Status status;
   MPI_Comm_rank(comm, &rank);
   MPI_Recv(&s, 1, MPI_INT, 0, 5, intercomm, &status);
   MPI_Reduce(&s, &r, 1, MPI_INT, MPI_SUM, 0, comm);
   if(rank==0) MPI_Send(&r, 1, MPI_INT, 0, 6, intercomm);
}
int main(int argc, char **argv)
{
   int rank, size, color, i, rank1;
   MPI_Comm newcomm, newintercomm;
   MPI_Init(&argc, &argv);
   MPI_Comm_size(MPI_COMM_WORLD, &size);
   MPI_Comm_rank(MPI_COMM_WORLD, &rank);
   color=(rank==0);
   MPI_Comm_split(MPI_COMM_WORLD, color, rank, &newcomm);
   MPI_Intercomm_create(newcomm, 0, MPI_COMM_WORLD, color, 1, &newintercomm);
   if(rank==0) master(newintercomm);
   else slave(newcomm, newintercomm);
   MPI_Comm_free(&newintercomm);
   MPI_Comm_free(&newcomm);
   MPI_Finalize();
}
