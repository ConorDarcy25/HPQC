#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>
#include <time.h>

void main_task(int uni_size, int source, int rank, int counter, int num_pings, long* vector, int size);
void client_task(int uni_size, int my_rank, int num_pings, int size);
void run_tasks(int uni_size, int my_rank, int num_pings, int counter, long* vector, int size);
void check_processors(int uni_size, int my_rank, int num_pings, int counter, long* vector, int size);
double to_second_float(struct timespec in_time);
struct timespec calculate_runtime(struct timespec start_time, struct timespec end_time);
void check_args(int argc, char **argv, int *num_pings, long *size);
void initialise_vector(long* vector, int size, int initial);
void print_vector(long* vector, int size);

int main(int argc, char **argv) 
{
    // declare and initialise error handling variable
    int ierror = 0;
    
    // declare and initialise rank and size variables
    int my_rank, uni_size, counter;
    my_rank = uni_size = counter = 0;
    
    // number of pings and array size from input
    int num_pings;
    long size;
    check_args(argc, argv, &num_pings, &size);
    
    // initialize MPI
    ierror = MPI_Init(&argc, &argv);
    
    // get the rank and world size
    ierror = MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);
    ierror = MPI_Comm_size(MPI_COMM_WORLD, &uni_size);
    
    // allocate memory for the vector
    long* my_vector = malloc(size * sizeof(long));
    // initialize the vector with the first element as zero
    initialise_vector(my_vector, size, 0);
    my_vector[0] = 0;
    
    // check if the number of processors is 2, then run the task
    check_processors(uni_size, my_rank, num_pings, counter, my_vector, size);
    
    // finalize MPI
    ierror = MPI_Finalize();
    
    // free allocated memory
    free(my_vector);
    
    return 0;
}

void main_task(int uni_size, int source, int rank, int counter, int num_pings, long* vector, int size)
{
    int tag = 0;
    MPI_Status status;
    
    long* send_message = malloc(size * sizeof(long));
    long* recv_message = malloc(size * sizeof(long));

    // Initialize the send and receive buffers
    initialise_vector(send_message, size, 0);
    initialise_vector(recv_message, size, 0);
    
    // Time tracking
    struct timespec start_time, end_time, time_diff;
    double runtime = 0.0;
    
    // Start time
    timespec_get(&start_time, TIME_UTC);

    // Sending and receiving messages
    if (vector[0] <= num_pings) {
        MPI_Send(vector, size, MPI_LONG, 1, tag, MPI_COMM_WORLD);
        MPI_Recv(recv_message, size, MPI_LONG, source, tag, MPI_COMM_WORLD, &status);
    }
    
    // End time
    timespec_get(&end_time, TIME_UTC);
    
    time_diff = calculate_runtime(start_time, end_time);
    runtime = to_second_float(time_diff);
    
    // Print time taken to send and receive
    // printf("Rank %d took %f to communicate.\n", rank, runtime);

    free(send_message);
    free(recv_message);
}

void client_task(int uni_size, int my_rank, int num_pings, int size)
{
    int dest = 0, tag = 0;
    MPI_Status status;
    
    long* send_message = malloc(size * sizeof(long));
    long* recv_message = malloc(size * sizeof(long));

    // Initialize the send and receive buffers
    initialise_vector(send_message, size, 0);
    initialise_vector(recv_message, size, 0);
    
    // Time tracking
    struct timespec start_time, end_time, time_diff;
    double runtime = 0.0;

    // Start time
    timespec_get(&start_time, TIME_UTC);
    
    // Receive the message
    MPI_Recv(recv_message, size, MPI_LONG, 0, tag, MPI_COMM_WORLD, &status);

    // Modify the first element of the array (the counter)
    send_message[0] = recv_message[0] + 1;

    // Send the message back
    if (send_message[0] <= num_pings) {
        MPI_Send(send_message, size, MPI_LONG, dest, tag, MPI_COMM_WORLD);
    }

    // End time
    timespec_get(&end_time, TIME_UTC);
    
    time_diff = calculate_runtime(start_time, end_time);
    runtime = to_second_float(time_diff);
    
    // Print time taken to send and receive
    // printf("Rank %d took %f to communicate.\n", my_rank, runtime);
    
    free(send_message);
    free(recv_message);
}

void run_tasks(int uni_size, int my_rank, int num_pings, int counter, long* vector, int size)
{
    int counters = 0;
    struct timespec start_time, end_time, time_diff;
    double runtime = 0.0;
    
    // Start time
    timespec_get(&start_time, TIME_UTC);

    while (counter < num_pings) {
        if (0 == my_rank) {
            main_task(uni_size, 1, my_rank, counter, num_pings, vector, size);
        } else {
            client_task(uni_size, my_rank, num_pings, size);
        }
        counter++;
        vector[0] = counter;
    }

    // End time
    timespec_get(&end_time, TIME_UTC);
    
    time_diff = calculate_runtime(start_time, end_time);
    runtime = to_second_float(time_diff);
    double avg_time = runtime / num_pings;
    if (0 == my_rank) {
        printf("The counter is at %d, with a total elapsed time of %lf and an average of %lf\n", counter, runtime, avg_time);
    }
}

void check_processors(int uni_size, int my_rank, int num_pings, int counter, long* vector, int size)
{
    if (uni_size == 2) {
        run_tasks(uni_size, my_rank, num_pings, counter, vector, size);
    } else {
        if (0 == my_rank) {
            printf("You can only use 2 processors for this program. MPI communicator size = %d\n", uni_size);
        }
    }
}

double to_second_float(struct timespec in_time)
{
    return in_time.tv_sec + in_time.tv_nsec / 1e9;
}

struct timespec calculate_runtime(struct timespec start_time, struct timespec end_time)
{
    struct timespec time_diff;
    long seconds = end_time.tv_sec - start_time.tv_sec;
    long nanoseconds = end_time.tv_nsec - start_time.tv_nsec;
    if (nanoseconds < 0) {
        seconds -= 1;
        nanoseconds += 1e9;
    }
    time_diff.tv_sec = seconds;
    time_diff.tv_nsec = nanoseconds;
    return time_diff;
}

void check_args(int argc, char **argv, int *num_pings, long *size)
{
    if (argc == 3) {
        *num_pings = atoi(argv[1]);
        *size = atoi(argv[2]);
    } else {
        fprintf(stderr, "ERROR: You did not provide the correct arguments!\n");
        fprintf(stderr, "Usage: %s <num_pings> <size_in_bytes>\n", argv[0]);
        exit(-1);
    }
}

void initialise_vector(long* vector, int size, int initial)
{
    for (int i = 0; i < size; i++) {
        vector[i] = initial;
    }
}

void print_vector(long* vector, int size)
{
    for (int i = 0; i < size; i++) {
        printf("%ld, ", vector[i]);
    }
}
