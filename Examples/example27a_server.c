#include <stdio.h>
#include "mpi.h"
int main(int argc, char **argv)
{
   int r;
   char port_name[MPI_MAX_PORT_NAME];
   MPI_Init(&argc, &argv);
   MPI_Status status;
   MPI_Comm intercomm;
   MPI_Open_port(MPI_INFO_NULL, port_name);
   MPI_Publish_name("example", MPI_INFO_NULL, port_name);
   MPI_Comm_accept(port_name, MPI_INFO_NULL, 0, MPI_COMM_SELF, &intercomm);
   MPI_Recv(&r, 1, MPI_INT, 0, 0, intercomm, &status);
   MPI_Unpublish_name("example", MPI_INFO_NULL, port_name);
   MPI_Close_port(port_name);
   printf("Client is sent %d\n", r);
   MPI_Finalize();
   return 0;
}
