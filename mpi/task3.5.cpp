#include <mpi.h>
#include <stdio.h>
#include <iostream>
#include <stdlib.h>

int main(int argc, char **argv)
{

    struct max_t {
        double value;
        int rank;
    };

    int rank;
    int size;
    int const N = 100;
    double* x;

    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    if (N % size != 0) {
        if (rank == 0) {
            printf("Amount of processes must be a divider of %d\n", N);
        }
        exit(1);
    }

    if (rank == 0) {
        x = new double[N];
        printf("x: [");
        for (int i = 0; i < N; i++) {
            x[i] = i;
            printf("%.2f ", x[i]);
        }
        printf("]\n");
    }

    int N_per_proc = N / size;
    double* x_per_proc = new double[N_per_proc];

    MPI_Scatter(x, N_per_proc, MPI_DOUBLE, x_per_proc, N_per_proc, MPI_DOUBLE, 0, MPI_COMM_WORLD);

    max_t global_max, local_max;
    local_max.rank = rank;
    local_max.value = x_per_proc[0];

    for (int i = 0; i < N_per_proc; i++) {
        if (x_per_proc[i] > local_max.value) {
            local_max.value = x_per_proc[i];
        }
    }

    MPI_Reduce(&local_max, &global_max, 1, MPI_DOUBLE_INT, MPI_MAXLOC, 0, MPI_COMM_WORLD);

    max_t* local_max_arr;
    if (rank == 0) {
        local_max_arr = new max_t[size];
    }
    MPI_Gather(&local_max, 1, MPI_DOUBLE_INT, local_max_arr, 1, MPI_DOUBLE_INT, 0, MPI_COMM_WORLD);

    if (rank == 0) {
        printf("max=%.2f in process %d of %d\n", global_max.value, global_max.rank, size);

        printf("Local maximums: ");
        for (int i = 0; i < size; i++) {
            printf("%.2f ", local_max_arr[i].value);
        }
        printf("\n");

        printf("Ranks: ");
        for (int i = 0; i < size; i++) {
            printf("%d ", local_max_arr[i].rank);
        }
        printf("\n");
    }
    MPI_Finalize();
    return 0;
}
