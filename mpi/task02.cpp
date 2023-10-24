#include <mpi.h>
#include <stdio.h>

int main(int argc, char **argv)
{
    int nPr, size;
    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &nPr);
    printf("Process %d size %d \n", nPr, size);
    MPI_Finalize();
    return 0;
}
