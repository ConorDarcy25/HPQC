#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>
#include <time.h>

void ping_pong(int num_pings, int rank);
double to_seconds(struct timespec start_time, struct timespec end_time);

int main(int argc, char **argv)
{
    int my_rank, uni_size;
    int num_pings = 0;
    
    if (argc == 2)
    {
        num_pings = atoi(argv[1]);
    }
    else
    {
        printf("Error: Expected usage: %s <num_pings>\n", argv[0]);
        return -1;
    }
    
    if (num_pings <= 0)
    {
        printf("Error: num_pings must be greater than zero\n");
        return -1;
    }
    
    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);
    MPI_Comm_size(MPI_COMM_WORLD, &uni_size);
    
    if (uni_size != 2)
    {
        if (my_rank == 0)
        {
            printf("This program requires exactly 2 processes, but got %d\n", uni_size);
        }
        MPI_Finalize();
        return -1;
    }
    
    ping_pong(num_pings, my_rank);
    
    MPI_Finalize();
    return 0;
}

void ping_pong(int num_pings, int rank)
{
    int counter = 0;
    int tag = 0;
    MPI_Status status;
    struct timespec start_time, end_time;
    
    if (rank == 0)
    {
        timespec_get(&start_time, TIME_UTC);
        while (counter < num_pings)
        {
            printf("Rank 0: Sending counter %d to Rank 1\n", counter);
            MPI_Send(&counter, 1, MPI_INT, 1, tag, MPI_COMM_WORLD);
            MPI_Recv(&counter, 1, MPI_INT, 1, tag, MPI_COMM_WORLD, &status);
            counter++;
        }
        timespec_get(&end_time, TIME_UTC);
        double elapsed_time = to_seconds(start_time, end_time);
        double avg_time = elapsed_time / num_pings;
        printf("Ping-Pong done: %d exchanges, Total time: %lf s, Avg time: %lf s\n", num_pings, elapsed_time, avg_time);
    }
    else
    {
        while (counter < num_pings)
        {
            MPI_Recv(&counter, 1, MPI_INT, 0, tag, MPI_COMM_WORLD, &status);
            counter++;
            MPI_Send(&counter, 1, MPI_INT, 0, tag, MPI_COMM_WORLD);
        }
    }
}

double to_seconds(struct timespec start_time, struct timespec end_time)
{
    double total_time = 0.0;
    total_time = (end_time.tv_sec - start_time.tv_sec) + (end_time.tv_nsec - start_time.tv_nsec) / 1e9;
    return total_time;
}
