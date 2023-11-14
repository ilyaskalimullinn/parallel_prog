#include <mpi.h>
#include <stdio.h>
#include <iostream>
#include <stdlib.h>

int main(int argc, char **argv)
{
    int rank;
    int size;
    int const N = 100;
    double* x;
    double *y;

    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    if (N % size != 0) {
        if (rank == 0) {
            printf("Amount of processes must be a divider of %d\n", N);
        }
        exit(1);
    }

    int N_per_proc = N / size;
    double* x_per_proc;
    double* y_per_proc;

    if (rank == 0) {
        x = new double[N];
        y = new double[N];
        printf("x: [");
        for (int i = 0; i < N; i++) {
            x[i] = i;
            printf("%.2f ", x[i]);
        }
        printf("]\n");

        printf("y: [");
        for (int i = 0; i < N; i++) {
            y[i] = i*2;
            printf("%.2f ", y[i]);
        }
        printf("]\n");
    }

    x_per_proc = new double[N_per_proc];
    y_per_proc = new double[N_per_proc];

    MPI_Scatter(x, N_per_proc, MPI_DOUBLE, x_per_proc, N_per_proc, MPI_DOUBLE, 0, MPI_COMM_WORLD);
    MPI_Scatter(y, N_per_proc, MPI_DOUBLE, y_per_proc, N_per_proc, MPI_DOUBLE, 0, MPI_COMM_WORLD);

    double sum = 0;
    for (int i = 0; i < N_per_proc; i++) {
        sum += x_per_proc[i] * y_per_proc[i];
    }

    double sum_final = 0;
    MPI_Reduce(&sum, &sum_final, 1, MPI_DOUBLE, MPI_SUM, 0, MPI_COMM_WORLD);

    if (rank == 0) {
        printf("product = %.2f \n", sum_final);
    }
    MPI_Finalize();
    return 0;
}
