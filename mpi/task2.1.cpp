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
    int const N = 100;
    int const a = 1;
    int const b = 2;

    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    if (N % size != 0) {
        printf("Amount of processes must be a divider of %d\n", N);
        exit(1);
    }

    int N_per_proc = N / size;
    int *x;
    int *y;
    int *s;
    int *x_proc;
    int *y_proc;
    int *s_proc;

    if (rank == 0) {
        x = (int*) malloc(sizeof(int) * N);
        y = (int*) malloc(sizeof(int) * N);

        printf("a = %d, b = %d\n", a, b);
        std::cout << "x: [";
        for (int i = 0; i < N; i++) {
            x[i] = (rand() % 100);
            std::cout << x[i] << " ";
        }
        std::cout << "]\n\n y: [";
        for (int i = 0; i < N; i++) {
            y[i] = (rand() % 100);
            std::cout << y[i] << " ";
        }
        std::cout << "]\n\n";
    }

    x_proc = (int*) malloc(sizeof(int) * N_per_proc);
    y_proc = (int*) malloc(sizeof(int) * N_per_proc);
    s_proc = (int*) malloc(sizeof(int) * N_per_proc);
    MPI_Scatter(x, N_per_proc, MPI_INT, x_proc, N_per_proc, MPI_INT, 0, MPI_COMM_WORLD);
    MPI_Scatter(y, N_per_proc, MPI_INT, y_proc, N_per_proc, MPI_INT, 0, MPI_COMM_WORLD);

    for (int i = 0; i < N_per_proc; i++) {
        s_proc[i] = a * x_proc[i] + b * y_proc[i];
    }

    if (rank == 0) {
        s = (int*) malloc(sizeof(int) * N);
    }

    MPI_Gather(s_proc, N_per_proc, MPI_INT, s, N_per_proc, MPI_INT, 0, MPI_COMM_WORLD);

    if (rank == 0) {
        std::cout << "s: [";
        for (int i = 0; i < N; i++) {
            std::cout << s[i] << " ";
        }
        std::cout << "]\n";
    }
    
    MPI_Finalize();
    return 0;
}
