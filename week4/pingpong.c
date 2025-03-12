#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>
#include <time.h>

int main_task(int uni_size, int source, int rank, int counter, int num_pings);
void client_task(int uni_size, int my_rank, int num_pings);
void run_tasks(int uni_size, int my_rank, int num_pings, int counter);
void check_processors(int uni_size, int my_rank, int num_pings, int counter);
double convert_to_seconds(struct timespec start, struct timespec end);
int validate_args(int argc, char **argv);

int main(int argc, char **argv) 
{
    int ierror = 0;
    int my_rank, uni_size, counter = 0;
    
    int num_pings = validate_args(argc, argv);
    
    ierror = MPI_Init(&argc, &argv);
    ierror = MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);
    ierror = MPI_Comm_size(MPI_COMM_WORLD, &uni_size);
    
    check_processors(uni_size, my_rank, num_pings, counter);
    
    ierror = MPI_Finalize();
    return 0;
}

int main_task(int uni_size, int source, int rank, int counter, int num_pings)
{
    int recv_message = 0, send_message = counter, count = 1, tag = 0;
    MPI_Status status;
    struct timespec start_time, end_time;
    timespec_get(&start_time, TIME_UTC);

    if (send_message <= num_pings) {
        MPI_Send(&send_message, count, MPI_INT, 1, tag, MPI_COMM_WORLD);
        MPI_Recv(&recv_message, count, MPI_INT, source, tag, MPI_COMM_WORLD, &status);
    }
    timespec_get(&end_time, TIME_UTC);
    
    double runtime = convert_to_seconds(start_time, end_time);
    return recv_message;
}

void client_task(int uni_size, int my_rank, int num_pings)
{
    int send_message = 0, recv_message = 0, count = 1, tag = 0;
    MPI_Status status;
    struct timespec start_time, end_time;
    timespec_get(&start_time, TIME_UTC);
    
    MPI_Recv(&recv_message, count, MPI_INT, 0, tag, MPI_COMM_WORLD, &status);
    send_message = recv_message + 1;
    
    if (send_message <= num_pings) {
        MPI_Send(&send_message, count, MPI_INT, 0, tag, MPI_COMM_WORLD);
    }
    
    timespec_get(&end_time, TIME_UTC);
    convert_to_seconds(start_time, end_time);
}

void run_tasks(int uni_size, int my_rank, int num_pings, int counter)
{
    struct timespec start_time, end_time;
    timespec_get(&start_time, TIME_UTC);

    while (counter < num_pings) {
        if (my_rank == 0) {
            for (int their_rank = 1; their_rank < uni_size; their_rank++) {
                main_task(uni_size, their_rank, my_rank, counter, num_pings);           
            }
        } else {
            client_task(uni_size, my_rank, num_pings);
        }
        counter++;
    }
    
    timespec_get(&end_time, TIME_UTC);
    double runtime = convert_to_seconds(start_time, end_time);
    if (my_rank == 0) {
        printf("Total elapsed time: %lf seconds\n", runtime);
    }
}

void check_processors(int uni_size, int my_rank, int num_pings, int counter)
{
    if (uni_size == 2) {
        run_tasks(uni_size, my_rank, num_pings, counter);
    } else if (my_rank == 0) {
        printf("Error: This program requires exactly 2 processors.\n");
    }
}

double convert_to_seconds(struct timespec start, struct timespec end)
{
    return (end.tv_sec - start.tv_sec) + (end.tv_nsec - start.tv_nsec) / 1e9;
}

int validate_args(int argc, char **argv)
{
    if (argc == 2) {
        return atoi(argv[1]);
    } else {
        fprintf(stderr, "Usage: %s [NUMBER]\n", argv[0]);
        exit(-1);
    }
}
