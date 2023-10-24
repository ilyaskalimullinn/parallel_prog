#include <mpi.h>
#include <stdio.h>
#include <iostream>

int main(int argc, char **argv)
{
    // random seed
    srand(time(0));

    int process;
    int N = 10;
    int a[N];
    MPI_Status status;


    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &process);

    if (process == 0) {
        for (int i = 0; i < N; i++) {
            a[i] = (rand() % 100);
        }

        MPI_Send(a, N, MPI_INT, 1, 0, MPI_COMM_WORLD);
    } else if (process == 1) {
        MPI_Recv(a, N, MPI_INT, 0, 0, MPI_COMM_WORLD, &status);

        for (int i = 0; i < N; i++) {
            std::cout << a[i] << " ";
        }
        std::cout << std::endl;
    }
    
    MPI_Finalize();
    return 0;
}
