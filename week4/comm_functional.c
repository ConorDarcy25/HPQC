#include <stdio.h>
#include <mpi.h>

// Function prototypes
void initialize_mpi(int *argc, char ***argv, int *my_rank, int *uni_size);
void finalize_mpi();
void send_message(int my_rank);
void receive_messages(int my_rank, int uni_size);
void handle_single_process(int uni_size);

int main(int argc, char **argv) 
{
    int my_rank, uni_size;
    
    initialize_mpi(&argc, &argv, &my_rank, &uni_size);
    
    if (uni_size > 1) {
        if (my_rank == 0) {
            receive_messages(my_rank, uni_size);
        } else {
            send_message(my_rank);
        }
    } else {
        handle_single_process(uni_size);
    }
    
    finalize_mpi();
    return 0;
}

void initialize_mpi(int *argc, char ***argv, int *my_rank, int *uni_size) {
    MPI_Init(argc, argv);
    MPI_Comm_rank(MPI_COMM_WORLD, my_rank);
    MPI_Comm_size(MPI_COMM_WORLD, uni_size);
}

void finalize_mpi() {
    MPI_Finalize();
}

void send_message(int my_rank) {
    int send_message = my_rank * 10;
    int dest = 0;  // Sending to rank 0
    int count = 1, tag = 0;
    
    MPI_Send(&send_message, count, MPI_INT, dest, tag, MPI_COMM_WORLD);
    printf("Hello, I am %d. Sent %d to Rank %d\n", my_rank, send_message, dest);
}

void receive_messages(int my_rank, int uni_size) {
    int recv_message, source;
    int count = 1, tag = 0;
    MPI_Status status;
    
    for (int their_rank = 1; their_rank < uni_size; their_rank++) {
        source = their_rank;
        MPI_Recv(&recv_message, count, MPI_INT, source, tag, MPI_COMM_WORLD, &status);
        printf("Hello, I am %d. Received %d from Rank %d\n", my_rank, recv_message, source);
    }
}

void handle_single_process(int uni_size) {
    printf("Unable to communicate with less than 2 processes. MPI communicator size = %d\n", uni_size);
}
