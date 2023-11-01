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
    int const M = 12;
    int const N = 10;
    int *A;
    int* B;
    int *C;
    int *A_proc;
    int *B_proc;
    int *C_proc;


    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    if (M % size != 0) {
        printf("Amount of processes must be a divider of %d\n", M);
        exit(1);
    }

    int nproc = N * M / size;

    if (rank == 0) {
        A = (int*) malloc(sizeof(int) * M * N);
        B = (int*) malloc(sizeof(int) * M * N);

        std::cout << "A:\n";
        for (int i = 0; i < M * N; i += N) {
            for (int j = 0; j < N; j++) {
                A[i + j] = (i + j) % 9;
                std::cout << A[i + j] << " ";
            }
            std::cout << std::endl;
        }
        std::cout << std::endl;

        std::cout << "B:\n";
        for (int i = 0; i < M * N; i += N) {
            for (int j = 0; j < N; j++) {
                B[i + j] = (i + j) % 7;
                std::cout << B[i + j] << " ";
            }
            std::cout << std::endl;
        }
        std::cout << std::endl;
    }

    A_proc = (int*) malloc(sizeof(int) * nproc);
    B_proc = (int*) malloc(sizeof(int) * nproc);
    C_proc = (int*) malloc(sizeof(int) * nproc);
    MPI_Scatter(A, nproc, MPI_INT, A_proc, nproc, MPI_INT, 0, MPI_COMM_WORLD);
    MPI_Scatter(B, nproc, MPI_INT, B_proc, nproc, MPI_INT, 0, MPI_COMM_WORLD);

    for (int i = 0; i < nproc; i += 1) {
        C_proc[i] = A_proc[i] * B_proc[i];
    }

    if (rank == 0) {
        C = (int*) malloc(sizeof(int) * N * M);
    }

    MPI_Gather(C_proc, nproc, MPI_INT, C, nproc, MPI_INT, 0, MPI_COMM_WORLD);

    if (rank == 0) {
        std::cout << "C:\n";
        for (int i = 0; i < M * N; i += N) {
            for (int j = 0; j < N; j++) {
                std::cout << C[i + j] << " ";
            }
            std::cout << std::endl;
        }
        std::cout << std::endl;
    }
    
    MPI_Finalize();
    return 0;
}
