#include <stdio.h>
#include "mpi.h"
#define N 1000
#define MAXPROC 128
void slave(double *a, int n, int rank, int size)
{
    int i, ibeg, iend, h;
    h = n / size;
    ibeg = h*rank;
    iend = (h+1)*rank;
    for (i = ibeg; i<iend;i++)
    {
        a[i] = rank;
    }
/* обработка локальной части массива a */
}
void master(double *a, int n)
{
    int i;
    for (i = 0; i<n;i++)
    {
        printf ("%lf ",a[i]);
    }
    
/* обработка массива a */
}
int main(int argc, char **argv)
{  int rank, size, num, k, i, indices[MAXPROC], source;
   MPI_Request req[MAXPROC];
   MPI_Status statuses[MAXPROC];
   double a[N][MAXPROC];
   MPI_Init(&argc, &argv);
   MPI_Comm_size(MPI_COMM_WORLD, &size);
   MPI_Comm_rank(MPI_COMM_WORLD, &rank);
   if(rank!=0)
      {
         slave((double*)a, N, rank, size);
         MPI_Send(a, N, MPI_DOUBLE, 0, 5, MPI_COMM_WORLD);
      }
      else
      { for(i = 0; i<size-1; i++)
               MPI_Irecv(&a[0][i], N, MPI_DOUBLE, i, 5, MPI_COMM_WORLD, &req[i]);
         
            MPI_Waitsome(size-1, req, &num, indices, statuses);
            for (i = 0; i< num; i++){
               source = statuses[i].MPI_SOURCE;
               master(&a[0][source], N);
               MPI_Irecv(&a[0][source], N, MPI_DOUBLE, source, 5, MPI_COMM_WORLD, &req[source]);
            
         }
      }
   MPI_Finalize();
   return 0;
}
