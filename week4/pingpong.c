#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>
#include <time.h>

void ping_pong(int num_pings, int my_rank);
double to_seconds(struct timespec start, struct timespec end);

int main(int argc, char **argv) 
{
    int my_rank, uni_size, num_pings;
    
    if (argc != 2) {
        printf("Error: Expected usage: %s <num_pings>\n", argv[0]);
        return -1;
    }
    num_pings = atoi(argv[1]);
    if (num_pings <= 0) {
        printf("Error: num_pings must be greater than zero\n");
        return -1;
    }
    
    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);
    MPI_Comm_size(MPI_COMM_WORLD, &uni_size);
    
    if (uni_size != 2) {
        if (my_rank == 0) {
            printf("This program needs exactly 2 processes to work, but got %d\n", uni_size);
        }
        MPI_Finalize();
        return -1;
    }
    
    ping_pong(num_pings, my_rank);
    
    MPI_Finalize();
    return 0;
}

void ping_pong(int num_pings, int my_rank) {
    int counter = 0;
    int tag = 0;
    MPI_Status status;
    struct timespec start_time, end_time;
    
    if (my_rank == 0) {
        timespec_get(&start_time, TIME_UTC);
        while (counter < num_pings) {
            printf("Rank 0: Sending counter value %d to Rank 1\n", counter);
            MPI_Send(&counter, 1, MPI_INT, 1, tag, MPI_COMM_WORLD);
            MPI_Recv(&counter, 1, MPI_INT, 1, tag, MPI_COMM_WORLD, &status);
            printf("Rank 0: Received counter value %d from Rank 1\n", counter);
            counter++;
        }
        timespec_get(&end_time, TIME_UTC);
        double elapsed_time = to_seconds(start_time, end_time);
        double avg_time = elapsed_time / num_pings;
        printf("Ping-Pong completed: %d pings, Total time: %lf s, Avg time: %lf s\n", num_pings, elapsed_time, avg_time);
    } else {
        while (counter < num_pings) {
            MPI_Recv(&counter, 1, MPI_INT, 0, tag, MPI_COMM_WORLD, &status);
            printf("Rank 1: Received counter value %d from Rank 0\n", counter);
            counter++;
            MPI_Send(&counter, 1, MPI_INT, 0, tag, MPI_COMM_WORLD);
            printf("Rank 1: Sent counter value %d to Rank 0\n", counter);
        }
    }
}

double to_seconds(struct timespec start, struct timespec end) {
    return (end.tv_sec - start.tv_sec) + (end.tv_nsec - start.tv_nsec) / 1e9;
}
