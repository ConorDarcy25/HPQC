#include <stdio.h>
#include <mpi.h>

// Function prototypes
void main_task(int uni_size, int source, int rank);
void client_task(int uni_size, int my_rank);
void run_tasks(int uni_size, int my_rank);
void check_processors(int uni_size, int my_rank);

int main(int argc, char **argv) 
{
    // declare and initialise rank and size variables
    int my_rank, uni_size;
    
    // initialize MPI
    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);
    MPI_Comm_size(MPI_COMM_WORLD, &uni_size);
    
    // checks the number of processors and runs tasks if valid
    check_processors(uni_size, my_rank);
    
    // finalize MPI
    MPI_Finalize();
    return 0;
}

void main_task(int uni_size, int source, int rank)
{
    int recv_message, count = 1, tag = 0;
    MPI_Status status;
    
    // receives the message
    MPI_Recv(&recv_message, count, MPI_INT, source, tag, MPI_COMM_WORLD, &status);
    
    // prints the message from the sender
    printf("Hello, I am %d of %d. Received %d from Rank %d\n", rank, uni_size, recv_message, source);
}

void client_task(int uni_size, int my_rank)
{
    int send_message = my_rank * 10;
    int dest = 0, count = 1, tag = 0;
    
    // sends the message
    MPI_Send(&send_message, count, MPI_INT, dest, tag, MPI_COMM_WORLD);
    
    // prints the message from the sender
    printf("Hello, I am %d of %d. Sent %d to Rank %d\n", my_rank, uni_size, send_message, dest);
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
        // prints a warning
        printf("Unable to communicate with less than 2 processes. MPI communicator size = %d\n", uni_size);
    }
}
