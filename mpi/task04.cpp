#include <mpi.h>
#include <stdio.h>
#include <iostream>

int main(int argc, char **argv)
{
    // random seed
    srand(time(0));

    int const MAIN_PR = 1;
    int rank;
    MPI_Status status;


    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    if (rank == MAIN_PR) {
        int N = 10;
        int a[N];
        for (int i = 0; i < N; i++) {
            a[i] = (rand() % 100);
        }

        int n_pr;
        MPI_Comm_size(MPI_COMM_WORLD, &n_pr);
        for (int pr = 0; pr < n_pr; pr++) {
            if (pr == MAIN_PR) {
                continue;
            }
            MPI_Send(a, N, MPI_INT, pr, 0, MPI_COMM_WORLD);
        }
        
    } else {
        int count;

        MPI_Probe(MAIN_PR, 0, MPI_COMM_WORLD, &status);
        MPI_Get_count(&status, MPI_INT, &count);

        int a[count];
        MPI_Recv(a, count, MPI_INT, MAIN_PR, 0, MPI_COMM_WORLD, &status);

        for (int i = 0; i < count; i++) {
            std::cout << a[i] << " ";
        }
        std::cout << std::endl;
    }
    
    MPI_Finalize();
    return 0;
}
