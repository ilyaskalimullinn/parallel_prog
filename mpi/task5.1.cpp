#include <mpi.h>
#include <stdio.h>
#include <iostream>
#include <stdlib.h>
#include <string>


int main(int argc, char **argv)
{
    int size_old, rank_old, size_new, rank_new;
    size_new = 5;
    int ranks[] = {8, 3, 9, 1, 6};

    const int N = 10;

    // random seed
    srand(time(0));

    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &size_old);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank_old);

    if (size_old != 10) {
        printf("Must be exactly 10 processes\n");
        MPI_Abort(MPI_COMM_WORLD, 1);
    }


    MPI_Group group_world, group_new;
    MPI_Comm comm_new;
    MPI_Comm_group(MPI_COMM_WORLD, &group_world);
    MPI_Group_incl(group_world, size_new, ranks, &group_new);
    MPI_Comm_create(MPI_COMM_WORLD, group_new, &comm_new);


    if (comm_new != MPI_COMM_NULL) {
        MPI_Comm_rank(comm_new, &rank_new);

        double a[N];

        if (rank_new == 0) {
            printf("Array ");
            for (int i = 0; i < N; i++) {
                a[i] = rand() % 1000;
                printf("%f ", a[i]);
            }
            printf("\n");
        }
        

        MPI_Bcast(a, N, MPI_DOUBLE, 0, comm_new);


        std::string out = "New rank %d, old rank %d, array ";

        for(auto i : a) {
            out += std::to_string(i);
            out.push_back(' ');
        }
        out.push_back('\n');
        
        printf(out.c_str(), rank_new, rank_old);

        if (rank_new == size_new - 1) {
            MPI_Send(a, N, MPI_DOUBLE, 0, 0, MPI_COMM_WORLD);
        }
    }

    if (rank_old == 0) {
        std::string out = "Old rank 0, array ";

        double b[N];

        MPI_Status status;
        MPI_Recv(b, N, MPI_DOUBLE, ranks[size_new - 1], 0, MPI_COMM_WORLD, &status);

        for(auto i : b) {
            out += std::to_string(i);
            out.push_back(' ');
        }
        out.push_back('\n');
        
        std::cout << out.c_str();
    }

    
    
    MPI_Group_free(&group_world);
    MPI_Group_free(&group_new);
    if (comm_new != MPI_COMM_NULL) {
        MPI_Comm_free(&comm_new);
    }

    MPI_Finalize();

}
