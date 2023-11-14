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
    double* b = new double[N];
    double *c;
    double *A_proc;
    double *c_proc;
    MPI_Status status;


    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    if (M % (size - 1) != 0) {
        printf("(Amount of processes - 1) must be a divider of %d\n", M);
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

        for (int r = 1; r < size; r++) {
            MPI_Send(b, N, MPI_DOUBLE, r, 0, MPI_COMM_WORLD);
            int idx = (r - 1) * c_size_proc;
            MPI_Send(&A[idx], A_size_proc, MPI_DOUBLE, r, 1, MPI_COMM_WORLD);
        }

        double c[M];
        for (int r = 1; r < size; r++) {
            int idx = (r - 1) * c_size_proc;
            MPI_Recv(&c[idx], c_size_proc, MPI_DOUBLE, r, 2, MPI_COMM_WORLD, &status);
        }

        printf("c: [");
        for (int i = 0; i < M; i++) {
            printf("%.2f ", c[i]);
        }
        printf("]\n");
    } else {
        double b_proc[M];
        double A_proc[c_size_proc][N];

        MPI_Recv(b_proc, M, MPI_DOUBLE, 0, 0, MPI_COMM_WORLD, &status);
        MPI_Recv(A_proc, A_size_proc, MPI_DOUBLE, 0, 1, MPI_COMM_WORLD, &status);

        double c_proc[c_size_proc];

        for (int i = 0; i < c_size_proc; i++) {
            c_proc[i] = 0;
            for (int j = 0; j < N; j++) {
                c_proc[i] += A_proc[i][j] * b_proc[j];
            }
        }

        MPI_Send(c_proc, c_size_proc, MPI_DOUBLE, 0, 2, MPI_COMM_WORLD);
    }
    
    MPI_Finalize();
    return 0;
}
