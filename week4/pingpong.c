#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>
#include <time.h>

int main_task(int uni_size, int source, int rank, int counter, int num_pings);
void client_task(int uni_size, int my_rank, int num_pings);
void run_tasks(int uni_size, int my_rank, int num_pings, int counter);
void check_processors(int uni_size, int my_rank, int num_pings, int counter);
double to_second_float(struct timespec in_time);
struct timespec calculate_runtime(struct timespec start_time, struct timespec end_time);
int check_args(int argc, char **argv);

int main(int argc, char **argv) 
{
    int ierror = 0;
    int my_rank, uni_size, counter;
    my_rank = uni_size = counter = 0;
    
    int num_pings = check_args(argc, argv);
    
    ierror = MPI_Init(&argc, &argv);
    ierror = MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);
    ierror = MPI_Comm_size(MPI_COMM_WORLD, &uni_size);
    
    check_processors(uni_size, my_rank, num_pings, counter);
    
    ierror = MPI_Finalize();
    return 0;
}

int main_task(int uni_size, int source, int rank, int counter, int num_pings)
{
    int recv_message, send_message, count, tag;
    recv_message = tag = 0;
    send_message = counter;
    count = 1;
    MPI_Status status;
    
    struct timespec start_time, end_time, time_diff;
    double runtime = 0.0;
    timespec_get(&start_time, TIME_UTC);
    
    if (send_message <= num_pings) {
        MPI_Send(&send_message, count, MPI_INT, 1, tag, MPI_COMM_WORLD);
        MPI_Recv(&recv_message, count, MPI_INT, source, tag, MPI_COMM_WORLD, &status);
    }
    timespec_get(&end_time, TIME_UTC);
    
    time_diff = calculate_runtime(start_time, end_time);
    runtime = to_second_float(time_diff);
    
    return recv_message;
}

void client_task(int uni_size, int my_rank, int num_pings)
{
    int send_message, recv_message, count, dest, tag;
    send_message = recv_message = dest = tag = 0;
    count = 1;
    MPI_Status status;
    
    struct timespec start_time, end_time, time_diff;
    double runtime = 0.0;
    timespec_get(&start_time, TIME_UTC);
    
    MPI_Recv(&recv_message, count, MPI_INT, 0, tag, MPI_COMM_WORLD, &status);
    send_message = recv_message + 1;
    
    if (send_message <= num_pings) {
        MPI_Send(&send_message, count, MPI_INT, 0, tag, MPI_COMM_WORLD);
    }
    timespec_get(&end_time, TIME_UTC);
    time_diff = calculate_runtime(start_time, end_time);
    runtime = to_second_float(time_diff);
}

void run_tasks(int uni_size, int my_rank, int num_pings, int counter)
{
    struct timespec start_time, end_time, time_diff;
    double runtime = 0.0;
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
    time_diff = calculate_runtime(start_time, end_time);
    runtime = to_second_float(time_diff);
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

double to_second_float(struct timespec in_time)
{
    return in_time.tv_sec + in_time.tv_nsec / 1e9;
}

struct timespec calculate_runtime(struct timespec start_time, struct timespec end_time)
{
    struct timespec time_diff;
    long int seconds, nanoseconds;
    seconds = end_time.tv_sec - start_time.tv_sec;
    nanoseconds = end_time.tv_nsec - start_time.tv_nsec;

    if (nanoseconds < 0) {
        seconds -= 1;
        nanoseconds += (long int)1e9;
    }
    
    time_diff.tv_sec = seconds;
    time_diff.tv_nsec = nanoseconds;
    return time_diff;
}

int check_args(int argc, char **argv)
{
    if (argc == 2) {
        return atoi(argv[1]);
    } else {
        fprintf(stderr, "Usage: %s [NUMBER]\n", argv[0]);
        exit(-1);
    }
}
