#include <mpi.h>
#include <stdio.h>
#include <iostream>
#include <stdlib.h>
#include <string>


int main(int argc, char **argv)
{
    int size;
    int rank;

    // random seed
    srand(time(0));

    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    if (rank == 0) {
        printf("size = %d\n", size);
    }

    const int N = 12;
    const int n_per_proc = N / size + 1;
    // ranks 0,1,...full_blocks_amount-1 will each receieve n_per_proc elements
    // rank = full_blocks_amount will receieve N % n_per_proc elements
    // ranks > full_blocks_amount will receive 0 elements
    const int full_blocks_amount = N / n_per_proc;

    const int n_last_proc = N % n_per_proc;

    int working_processes_amount = full_blocks_amount;
    if (n_last_proc != 0) {
        working_processes_amount += 1;
    }

    int n_receive;
    

    if (rank < full_blocks_amount) {
        n_receive = n_per_proc;
    } else if (rank == full_blocks_amount) {
        n_receive = N % n_per_proc;
    } else {
        n_receive = 0;
    }

    int a_per_proc[n_receive];


    if (rank == 0) {
        int a[N];
        printf("a = [");
        for (int i = 0; i < N; i++) {
            a[i] = rand() % 1000;
            printf("%d ", a[i]);
        }
        printf("]\n");

        // makes sense to only send this to processes 0...full_blocks_amount-1 at first
        int idx = 0;
        for (int r = 0; r < full_blocks_amount; r++) {
            MPI_Send(&a[idx], n_per_proc, MPI_INT, r, 0, MPI_COMM_WORLD);
            idx += n_per_proc;
        }
        // now send to process full_blocks_amount but only if it receives at least one element
        if (n_last_proc != 0) {
            MPI_Send(&a[idx], n_last_proc, MPI_INT, full_blocks_amount, 0, MPI_COMM_WORLD);
        }
    }
    
    if (n_receive > 0) {
        int a_received[n_receive];

        MPI_Status status;
        MPI_Recv(a_received, n_receive, MPI_INT, 0, 0, MPI_COMM_WORLD, &status);
        
        std::string out = "Rank %d, array ";


        for(auto i : a_received) {
            out += std::to_string(i);
            out.push_back(' ');
        }
        out.push_back('\n');
        
        printf(out.c_str(), rank);
    }

    MPI_Finalize();

}
