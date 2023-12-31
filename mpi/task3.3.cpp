#include <mpi.h>
#include <stdio.h>
#include <iostream>
#include <stdlib.h>

int main(int argc, char **argv)
{
    int rank;
    int size;
    int const M = 12;
    int const N = 8;
    double *A;
    double b[N];
    double *c;
    double *A_proc;
    double *c_proc;


    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    if (M % size != 0) {
        printf("Amount of processes must be a divider of %d\n", M);
        exit(1);
    }

    int c_size_proc = M / size;
    int A_size_proc = c_size_proc * N;

    if (rank == 0) {
        A = new double[M * N];

        std::cout << "A:\n";
        for (int i = 0; i < M * N; i += N) {
            for (int j = 0; j < N; j++) {
                A[i + j] = (i + j) % 5;
                printf("%.2f ", A[i + j]);
            }
            std::cout << std::endl;
        }
        std::cout << std::endl;

        std::cout << "b: [";
        for (int i = 0; i < N; i++) {
            b[i] = i % 3;
            printf("%.2f ", b[i]);
        }
        std::cout << "]\n\n";
    }

    A_proc = new double[A_size_proc];
    c_proc = new double[c_size_proc];
    MPI_Scatter(A, A_size_proc, MPI_DOUBLE, A_proc, A_size_proc, MPI_DOUBLE, 0, MPI_COMM_WORLD);
    MPI_Bcast(b, N, MPI_DOUBLE, 0, MPI_COMM_WORLD);

    for (int i = 0; i < c_size_proc; i += 1) {
        c_proc[i] = 0;
        for (int j = 0; j < N; j++) {
            c_proc[i] += A_proc[N * i + j] * b[j];
        }
    }

    if (rank == 0) {
        c = new double[M];
    }

    MPI_Gather(c_proc, c_size_proc, MPI_DOUBLE, c, c_size_proc, MPI_DOUBLE, 0, MPI_COMM_WORLD);

    if (rank == 0) {
        std::cout << "c: [";
        for (int i = 0; i < M; i++) {
            printf("%.2f ", c[i]);
        }
        std::cout << "]\n";
    }
    
    MPI_Finalize();
    return 0;
}
