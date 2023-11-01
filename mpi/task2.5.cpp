#include <mpi.h>
#include <stdio.h>
#include <iostream>
#include <stdlib.h>

int main(int argc, char **argv)
{
    // random seed
    srand(time(0));

    int rank;
    int size;
    int const M = 20;
    int const N = 10;
    int* A = (int*) malloc(sizeof(int) * M * N);
    int* B;
    int *B_proc;


    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    if (M % size != 0) {
        printf("Amount of processes must be a divider of %d\n", N);
        exit(1);
    }

    
    if (rank == 0) {
        std::cout << "A:\n";
        for (int i = 0; i < M * N; i += N) {
            for (int j = 0; j < N; j++) {
                A[i + j] = rand() % 10;
                std::cout << A[i + j] << " ";
            }
            std::cout << std::endl;
        }
        std::cout << std::endl;
    }
    B_proc = (int*) malloc(sizeof(int) * M * N / size); 
    MPI_Bcast(A, M * N, MPI_INT, 0, MPI_COMM_WORLD);
    int shift = N * rank  / size;
    for (int i = 0; i < N / size; i++) {
        for (int j = 0; j < M; j++) {
            B_proc[i * M + j] = A[j * N + i + shift];
        }
    }

    if (rank == 0) {
        B = (int*) malloc(sizeof(int) * N * M);
    }

    MPI_Gather(B_proc, M * N / size, MPI_INT, B, M * N / size, MPI_INT, 0, MPI_COMM_WORLD);

    if (rank == 0) {
        std::cout << "A.T:\n";
        for (int i = 0; i < N * M; i += M) {
            for (int j = 0; j < M; j++) {
                std::cout << B[i + j] << " ";
            }
            std::cout << std::endl;
        }
        std::cout << std::endl;
    }
    
    MPI_Finalize();
    return 0;
}
