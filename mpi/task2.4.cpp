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
    int const K = 15;
    int *A;
    int* B;
    int *C;
    int *A_proc;
    int *C_proc;


    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    if (M % size != 0) {
        printf("Amount of processes must be a divider of %d\n", M);
        exit(1);
    }

    B = (int*) malloc(sizeof(int) * N * K);
    if (rank == 0) {
        A = (int*) malloc(sizeof(int) * M * N);

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
        for (int i = 0; i < N * K; i += K) {
            for (int j = 0; j < K; j++) {
                B[i + j] = (i + j) % 7;
                std::cout << B[i + j] << " ";
            }
            std::cout << std::endl;
        }
        std::cout << std::endl;
    }

    A_proc = (int*) malloc(sizeof(int) * M * N / size);
    C_proc = (int*) malloc(sizeof(int) * M * K / size);

    MPI_Scatter(A, M * N / size, MPI_INT, A_proc, M * N / size, MPI_INT, 0, MPI_COMM_WORLD);
    MPI_Bcast(B, N * K, MPI_INT, 0, MPI_COMM_WORLD);

    for (int i = 0; i < M / size; i++) {
        for (int j = 0; j < K; j++) {
            int ind = i * K + j;
            C_proc[ind] = 0;
            for (int k = 0; k < N; k++) {
                C_proc[ind] += A_proc[i * N + k] * B[k * K + j];
            }
        }
    }

    if (rank == 0) {
        C = (int*) malloc(sizeof(int) * M * K);
    }

    MPI_Gather(C_proc, M * K / size, MPI_INT, C, M * K / size, MPI_INT, 0, MPI_COMM_WORLD);

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
