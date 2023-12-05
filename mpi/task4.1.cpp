#include <mpi.h>
#include <stdio.h>
#include <iostream>
#include <stdlib.h>


void one_way(int n_bytes, int rank, double *result)
{
    unsigned char bytes_to_send[n_bytes];
    unsigned char bytes_received[n_bytes];

    
    for (int i = 0; i < n_bytes; i++) {
        bytes_to_send[i] = 'a' + rand() % 26;
    }   

    MPI_Request send_request;
    MPI_Request receive_request;
    MPI_Status status;

    double start_send = MPI_Wtime();

    MPI_Isend(bytes_to_send, n_bytes, MPI_BYTE, 1 - rank, 0, MPI_COMM_WORLD, &send_request);
    MPI_Irecv(bytes_received, n_bytes, MPI_BYTE, 1 - rank, 0, MPI_COMM_WORLD, &receive_request);

    MPI_Wait(&send_request, &status);
    MPI_Wait(&receive_request, &status);
    double end_receive = MPI_Wtime();

    double end_send;

    MPI_Send(&end_receive, 1, MPI_DOUBLE, 1 - rank, 1, MPI_COMM_WORLD);
    MPI_Recv(&end_send, 1, MPI_DOUBLE, 1 - rank, 1, MPI_COMM_WORLD, &status);
    *result = end_send - start_send;
    
}

void two_way(int n_bytes, int rank, bool verbose, double *result)
{
    if (rank == 0) {
        unsigned char bytes_to_send[n_bytes];
        unsigned char bytes_received[n_bytes];

        if (verbose) {
            printf("Bytes to send: [");
            for (int i = 0; i < n_bytes; i++) {
                bytes_to_send[i] = 'a' + rand() % 26;
                std::cout << bytes_to_send[i] << " ";
            }
            printf("]\n");
        } else {
            for (int i = 0; i < n_bytes; i++) {
                bytes_to_send[i] = 'a' + rand() % 26;
            }
        }        

        double start = MPI_Wtime();

        MPI_Send(bytes_to_send, n_bytes, MPI_BYTE, 1, 0, MPI_COMM_WORLD);

        MPI_Status status;
        MPI_Recv(bytes_received, n_bytes, MPI_BYTE, 1, 1, MPI_COMM_WORLD, &status);

        double end = MPI_Wtime();

        if (verbose) {
            printf("Bytes received: [");
            for (int i = 0; i < n_bytes; i++) {
                std::cout << bytes_received[i] << " ";
            }
            printf("]\n");
        }

        *result = end - start;
    } else {
        unsigned char bytes[n_bytes];

        MPI_Status status;
        MPI_Recv(bytes, n_bytes, MPI_BYTE, 0, 0, MPI_COMM_WORLD, &status);

        MPI_Send(bytes, n_bytes, MPI_BYTE, 0, 1, MPI_COMM_WORLD);
    } 
}

int main(int argc, char **argv)
{
    int size;
    int rank;

    // random seed
    srand(time(0));

    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    if (size != 2) {
        printf("Must start with 2 processes\n");
        MPI_Abort(MPI_COMM_WORLD, 1);
    }

    double time;
    if (rank == 0) {
        printf("Two way PingPong\n");
    }
    for (int n = 10; n <= 1000000; n *= 10) {
        two_way(n, rank, false, &time);
        if (rank == 0) {
            printf("N bytes: %d, Time: %f\n", n, time);
        }
    }

    if (rank == 0) {
        printf("**************************\n");
    }


    if (rank == 0) {
        printf("One way PingPong\n");
    }
    for (int n = 10; n <= 1000000; n *= 10) {
        one_way(n, rank, &time);
        printf("N bytes: %d, Rank: %d, Time: %f\n", n, rank, time);
    }
    
    MPI_Finalize();
    
}
