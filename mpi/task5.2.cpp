#include <mpi.h>
#include <stdio.h>
#include <iostream>
#include <stdlib.h>
#include <string>


int main(int argc, char **argv)
{
    const int M = 8;
    const int N = M;

    int size;
    int rank;

    // random seed
    srand(time(0));

    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    if (size != 2) {
        printf("Must be exactly 2 processes\n");
        MPI_Abort(MPI_COMM_WORLD, 1);
    }


    if (rank == 0) {
        printf("Array\n");
        int a[M * N];
        for (int i = 0; i < M * N; i += N) {
            for (int j = 0; j < N; j++) {
                a[i + j] = rand() % 1000;
                printf("%d ", a[i+j]);
            }
            printf("\n");
        }

        MPI_Datatype skip_rows;
        MPI_Type_vector(M / 2, N, 2 * N, MPI_INT, &skip_rows);
        MPI_Type_commit(&skip_rows);

        MPI_Send(a, 1, skip_rows, 1, 0, MPI_COMM_WORLD);
        MPI_Send(&a[N], 1, skip_rows, 1, 1, MPI_COMM_WORLD);

        MPI_Type_free(&skip_rows);
    } else {
        int b[M * N / 2];
        int c[M * N / 2];

        MPI_Status status;
        MPI_Recv(b, M * N / 2, MPI_INT, 0, 0, MPI_COMM_WORLD, &status);
        MPI_Recv(c, M * N / 2, MPI_INT, 0, 1, MPI_COMM_WORLD, &status);

        printf("Received array (odd rows)\n");
        for (int i = 0; i < M * N / 2; i += N) {
            for (int j = 0; j < N; j++) {
                printf("%d ", b[i+j]);
            }
            printf("\n");
        }

        printf("Received array (even rows)\n");
        for (int i = 0; i < M * N / 2; i += N) {
            for (int j = 0; j < N; j++) {
                printf("%d ", c[i+j]);
            }
            printf("\n");
        }
        
    }

    MPI_Finalize();
}
