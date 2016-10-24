#include <stdio.h>
#include "mpi.h"
int main(int argc, char **argv)
{
   int rank, i, size, size1, n;
   MPI_Status status;
   MPI_Group group, group1, group2;
   int ranks[128], rank1, rank2, rank3;
   MPI_Init(&argc, &argv);
   MPI_Comm_size(MPI_COMM_WORLD, &size);
   MPI_Comm_rank(MPI_COMM_WORLD, &rank);
   MPI_Comm_group(MPI_COMM_WORLD, &group);
   size1 = size/2;
   for(i = 0; i<size1; i++) ranks[i] = i;
   MPI_Group_incl(group, size1, ranks, &group1);
   MPI_Group_excl(group, size1, ranks, &group2);
   MPI_Group_rank(group1, &rank1);
   MPI_Group_rank(group2, &rank2);
   if(rank1==MPI_UNDEFINED){
      if(!(size%2) || rank!=size-1) MPI_Group_translate_ranks(group1, 1, &rank2, group, &rank3);
      else rank3 = MPI_PROC_NULL;
   }
   else MPI_Group_translate_ranks(group2, 1, &rank1, group, &rank3);
   MPI_Sendrecv(&rank, 1, MPI_INT, rank3, 1, &n, 1, MPI_INT, rank3, 1, MPI_COMM_WORLD, &status);
   MPI_Group_free(&group);
   MPI_Group_free(&group1);
   MPI_Group_free(&group2);
   printf("process %d n=%d\n", rank, n);
   MPI_Finalize();
}
