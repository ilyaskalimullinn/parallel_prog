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

    if (size != 4) {
        printf("Must be exactly 4 processes\n");
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

        int blocklengths[] = {N, N};
        MPI_Aint displacements[] = {0, M * N / 2 * sizeof(int)}; 
        MPI_Datatype types[] = {MPI_INT, MPI_INT};
        MPI_Type_struct(2, blocklengths, displacements, types, &skip_rows);
        MPI_Type_commit(&skip_rows);

        for (int rank = 0; rank < 4; rank++) {
            MPI_Send(&a[rank * N], 1, skip_rows, rank, 0, MPI_COMM_WORLD);
        }

        MPI_Type_free(&skip_rows);
    }

    int b[2 * N];

    MPI_Status status;
    MPI_Recv(b, N * 2, MPI_INT, 0, 0, MPI_COMM_WORLD, &status);

    std::string out = "Rank %d, array\n";
    for (int i = 0; i < 2 * N; i += N) {
        for (int j = 0; j < N; j++) {
            out += std::to_string(b[i + j]);
            out.push_back(' ');
        }
        out.push_back('\n');
    }
    out.push_back('\n');
    printf(out.c_str(), rank);


    MPI_Finalize();
}
