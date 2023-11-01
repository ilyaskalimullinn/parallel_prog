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
    int const N = 12;
    int *A;
    int* b = (int*) malloc(sizeof(int) * N);
    int *c;
    int *A_proc;
    int *c_proc;


    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    if (N % size != 0) {
        printf("Amount of processes must be a divider of %d\n", N);
        exit(1);
    }

    int c_size_proc = N / size;
    int A_size_proc = c_size_proc * N;

    if (rank == 0) {
        A = (int*) malloc(sizeof(int) * N * N);

        std::cout << "A:\n";
        for (int i = 0; i < N * N; i += N) {
            for (int j = 0; j < N; j++) {
                A[i + j] = (i + j) % 5;
                std::cout << A[i + j] << " ";
            }
            std::cout << std::endl;
        }
        std::cout << std::endl;

        std::cout << "b: [";
        for (int i = 0; i < N; i++) {
            b[i] = i % 3;
            std::cout << b[i] << " ";
        }
        std::cout << "]\n\n";
    }

    A_proc = (int*) malloc(sizeof(int) * A_size_proc);
    c_proc = (int*) malloc(sizeof(int) * c_size_proc);
    MPI_Scatter(A, A_size_proc, MPI_INT, A_proc, A_size_proc, MPI_INT, 0, MPI_COMM_WORLD);
    MPI_Bcast(b, N, MPI_INT, 0, MPI_COMM_WORLD);

    for (int i = 0; i < c_size_proc; i += 1) {
        c_proc[i] = 0;
        for (int j = 0; j < N; j++) {
            c_proc[i] += A_proc[N * i + j] * b[j];
        }
    }

    if (rank == 0) {
        c = (int*) malloc(sizeof(int) * N);
    }

    MPI_Gather(c_proc, c_size_proc, MPI_INT, c, c_size_proc, MPI_INT, 0, MPI_COMM_WORLD);

    if (rank == 0) {
        std::cout << "c: [";
        for (int i = 0; i < N; i++) {
            std::cout << c[i] << " ";
        }
        std::cout << "]\n";
    }
    
    MPI_Finalize();
    return 0;
}
