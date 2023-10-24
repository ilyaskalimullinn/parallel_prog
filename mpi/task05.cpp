#include <mpi.h>
#include <stdio.h>
#include <iostream>

int main(int argc, char **argv)
{
    int rank;
    int num;
    MPI_Status status;

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    if (rank == 0) {

        int n_pr;
        MPI_Comm_size(MPI_COMM_WORLD, &n_pr);
        for (int pr = 1; pr < n_pr; pr++) {
            MPI_Recv(&num, 1, MPI_INT, pr, 0, MPI_COMM_WORLD, &status);
            std::cout << num << " ";
        }
        std::cout << std::endl;
        
    } else {
        num = rank * 10;
        MPI_Send(&num, 1, MPI_INT, 0, 0, MPI_COMM_WORLD);
    }
    
    MPI_Finalize();
    return 0;
}
