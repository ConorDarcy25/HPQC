#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>
#include <time.h>

void main_task(int uni_size, int source, int rank, int counter, int num_pings, int array_size);
void client_task(int uni_size, int my_rank, int num_pings, int array_size);
void run_tasks(int uni_size, int my_rank, int num_pings, int counter, int array_size);
void check_processors(int uni_size, int my_rank, int num_pings, int counter, int array_size);
double convert_to_seconds(struct timespec start, struct timespec end);
struct timespec calculate_runtime(struct timespec start_time, struct timespec end_time);
int validate_args(int argc, char **argv, int *array_size);
void initialize_array(int *array, int size, int initial_value);
void print_array(int *array, int size);

int main(int argc, char **argv) 
{
    int ierror = 0;
    int my_rank, uni_size, counter;
    my_rank = uni_size = counter = 0;
    int array_size = 0;
    
    int num_pings = validate_args(argc, argv, &array_size);
    
    ierror = MPI_Init(&argc, &argv);
    ierror = MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);
    ierror = MPI_Comm_size(MPI_COMM_WORLD, &uni_size);
    
    check_processors(uni_size, my_rank, num_pings, counter, array_size);
    
    ierror = MPI_Finalize();
    return 0;
}

void main_task(int uni_size, int source, int rank, int counter, int num_pings, int array_size)
{
    int *array = (int*)malloc(array_size * sizeof(int));
    if (array == NULL) {
        fprintf(stderr, "Memory allocation failed\n");
        MPI_Abort(MPI_COMM_WORLD, 1);
    }
    initialize_array(array, array_size, 0);
    array[0] = counter;
    
    MPI_Status status;
    struct timespec start_time, end_time;
    timespec_get(&start_time, TIME_UTC);
    
    if (array[0] <= num_pings) {
        MPI_Send(array, array_size, MPI_INT, 1, 0, MPI_COMM_WORLD);
        MPI_Recv(array, array_size, MPI_INT, source, 0, MPI_COMM_WORLD, &status);
    }
    
    timespec_get(&end_time, TIME_UTC);
    double runtime = convert_to_seconds(start_time, end_time);
    free(array);
}

void client_task(int uni_size, int my_rank, int num_pings, int array_size)
{
    int *array = (int*)malloc(array_size * sizeof(int));
    if (array == NULL) {
        fprintf(stderr, "Memory allocation failed\n");
        MPI_Abort(MPI_COMM_WORLD, 1);
    }
    
    initialize_array(array, array_size, 0);
    MPI_Status status;
    struct timespec start_time, end_time;
    timespec_get(&start_time, TIME_UTC);
    
    MPI_Recv(array, array_size, MPI_INT, 0, 0, MPI_COMM_WORLD, &status);
    array[0] += 1;
    
    if (array[0] <= num_pings) {
        MPI_Send(array, array_size, MPI_INT, 0, 0, MPI_COMM_WORLD);
    }
    
    timespec_get(&end_time, TIME_UTC);
    double runtime = convert_to_seconds(start_time, end_time);
    free(array);
}

void run_tasks(int uni_size, int my_rank, int num_pings, int counter, int array_size)
{
    struct timespec start_time, end_time;
    timespec_get(&start_time, TIME_UTC);
    
    while (counter < num_pings) {
        if (my_rank == 0) {
            for (int their_rank = 1; their_rank < uni_size; their_rank++) {
                main_task(uni_size, their_rank, my_rank, counter, num_pings, array_size);
            }
        } else {
            client_task(uni_size, my_rank, num_pings, array_size);
        }
        counter++;
    }
    
    timespec_get(&end_time, TIME_UTC);
    double runtime = convert_to_seconds(start_time, end_time);
    if (my_rank == 0) {
        printf("Total elapsed time: %lf seconds\n", runtime);
    }
}

void check_processors(int uni_size, int my_rank, int num_pings, int counter, int array_size)
{
    if (uni_size == 2) {
        run_tasks(uni_size, my_rank, num_pings, counter, array_size);
    } else if (my_rank == 0) {
        printf("Error: This program requires exactly 2 processors.\n");
    }
}

double convert_to_seconds(struct timespec start, struct timespec end)
{
    return (end.tv_sec - start.tv_sec) + (end.tv_nsec - start.tv_nsec) / 1e9;
}

struct timespec calculate_runtime(struct timespec start_time, struct timespec end_time)
{
    struct timespec time_diff;
    time_diff.tv_sec = end_time.tv_sec - start_time.tv_sec;
    time_diff.tv_nsec = end_time.tv_nsec - start_time.tv_nsec;
    
    if (time_diff.tv_nsec < 0) {
        time_diff.tv_sec -= 1;
        time_diff.tv_nsec += 1e9;
    }
    return time_diff;
}

int validate_args(int argc, char **argv, int *array_size)
{
    if (argc == 3) {
        *array_size = atoi(argv[2]);
        return atoi(argv[1]);
    } else {
        fprintf(stderr, "Usage: %s [NUMBER_OF_PINGS] [ARRAY_SIZE]\n", argv[0]);
        exit(-1);
    }
}

void initialize_array(int *array, int size, int initial_value)
{
    for (int i = 0; i < size; i++) {
        array[i] = initial_value;
    }
}

void print_array(int *array, int size)
{
    for (int i = 0; i < size; i++) {
        printf("%d, ", array[i]);
    }
    printf("\n");
}
