#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>

void main_task(int uni_size, int source, int rank);
void client_task(int uni_size, int my_rank);
void run_tasks(int uni_size, int my_rank);
void check_processors(int uni_size, int my_rank);

int main(int argc, char **argv) 
{
    // Initialise rank and variables
    int my_rank, uni_size;
    
    // Initialize MPI
    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);
    MPI_Comm_size(MPI_COMM_WORLD, &uni_size);
    
    // Checks the number of processors and runs tasks if valid
    check_processors(uni_size, my_rank);
    
    // Finalize MPI
    MPI_Finalize();
    return 0;
}

void main_task(int uni_size, int source, int rank)
{
    int recv_message, count = 1, tag = 0;
    MPI_Status status;
    
    // Receives the message
    MPI_Recv(&recv_message, count, MPI_INT, source, tag, MPI_COMM_WORLD, &status);
    
    // Prints the message
    printf("Hello, I am %d of %d. Received %d from Rank %d\n", rank, uni_size, recv_message, source);
}

void client_task(int uni_size, int my_rank)
{
    int send_message = my_rank * 10;
    int dest = 0, count = 1, tag = 0;
    MPI_Request request;
    
    // Standard Send
    MPI_Send(&send_message, count, MPI_INT, dest, tag, MPI_COMM_WORLD);
    printf("(MPI_Send) Hello, I am %d of %d. Sent %d to Rank %d\n", my_rank, uni_size, send_message, dest);
    
    // Synchronous Send
    MPI_Ssend(&send_message, count, MPI_INT, dest, tag, MPI_COMM_WORLD);
    printf("(MPI_Ssend) Hello, I am %d of %d. Sent %d to Rank %d\n", my_rank, uni_size, send_message, dest);
    
    // Buffered Send
    int buffer_size = sizeof(int) * count + MPI_BSEND_OVERHEAD;
    void *buffer = malloc(buffer_size);
    MPI_Buffer_attach(buffer, buffer_size);
    MPI_Bsend(&send_message, count, MPI_INT, dest, tag, MPI_COMM_WORLD);
    printf("(MPI_Bsend) Hello, I am %d of %d. Sent %d to Rank %d\n", my_rank, uni_size, send_message, dest);
    MPI_Buffer_detach(&buffer, &buffer_size);
    free(buffer);
    
    // Ready Send
    MPI_Rsend(&send_message, count, MPI_INT, dest, tag, MPI_COMM_WORLD);
    printf("(MPI_Rsend) Hello, I am %d of %d. Sent %d to Rank %d\n", my_rank, uni_size, send_message, dest);
    
    // Non-blocking Send
    MPI_Isend(&send_message, count, MPI_INT, dest, tag, MPI_COMM_WORLD, &request);
    MPI_Wait(&request, MPI_STATUS_IGNORE);
    printf("(MPI_Isend) Hello, I am %d of %d. Sent %d to Rank %d\n", my_rank, uni_size, send_message, dest);
}

void run_tasks(int uni_size, int my_rank)
{
    if (my_rank == 0) {
        for (int their_rank = 1; their_rank < uni_size; their_rank++) {
            main_task(uni_size, their_rank, my_rank);
        }
    } else {
        client_task(uni_size, my_rank);
    }
}

void check_processors(int uni_size, int my_rank)
{
    if (uni_size > 1) {
        run_tasks(uni_size, my_rank);
    } else {
        // Prints a warning
        printf("Cannot run with less than 2 processes. MPI communicator size = %d\n", uni_size);
    }
}
