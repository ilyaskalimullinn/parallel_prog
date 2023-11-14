#include <mpi.h>
#include <stdio.h>
#include <iostream>
#include <stdlib.h>

int main(int argc, char **argv)
{
    int rank;
    int size;
    int const M = 12;
    int const N = 10;
    double* A;


    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    if (M % size != 0) {
        printf("Amount of processes must be a divider of %d\n", M);
        exit(1);
    }

    if (rank == 0) {
        A = new double[M * N];

        std::cout << "A:\n";
        for (int i = 0; i < M * N; i += N) {
            for (int j = 0; j < N; j++) {
                A[i + j] = (i + j) % 3;
                printf("%.2f ", A[i + j]);
            }
            std::cout << std::endl;
        }
        std::cout << std::endl;
    }

    int M_per_proc = M / size;
    int n_per_proc = M_per_proc * N;
    double* A_per_proc = new double[n_per_proc];
    MPI_Scatter(A, n_per_proc, MPI_DOUBLE, A_per_proc, n_per_proc, MPI_DOUBLE, 0, MPI_COMM_WORLD);

    double row_sum;
    double sum = 0;
    for (int i = 0; i < M_per_proc * N; i += N) {
        row_sum = 0;
        for (int j = 0; j < N; j++) {
            row_sum += abs(A_per_proc[i + j]);
        }
        if (row_sum > sum) {
            sum = row_sum;
        }
    }

    double norm;

    MPI_Reduce(&sum, &norm, 1, MPI_DOUBLE, MPI_MAX, 0, MPI_COMM_WORLD);

    if (rank == 0) {
        printf("Norm = %.2f\n", norm);
    }

}
