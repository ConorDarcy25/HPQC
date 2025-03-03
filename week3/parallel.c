#include <stdio.h>
#include <stdlib.h>
#include <time.h>

// Function declarations
double to_second_float(struct timespec in_time);
struct timespec calculate_runtime(struct timespec start_time, struct timespec end_time);
int check_args(int argc, char **argv);
void initialise_vector(int vector[], int size, int initial);
void print_vector(int vector[], int size);
int parallel_sum_vector(int vector[], int size, int num_threads);
void non_trivial_vector(int vector[], int size);

int main(int argc, char **argv)
{
    int num_arg = check_args(argc, argv);
    struct timespec start_time, end_time, time_diff;
    double runtime = 0.0;

    // Get time before the sum 
    timespec_get(&start_time, TIME_UTC);

    int* my_vector = malloc(num_arg * sizeof(int));
    initialise_vector(my_vector, num_arg, 0);
    non_trivial_vector(my_vector, num_arg);

    int num_threads = 4; // Set a fixed number of threads
    int my_sum = parallel_sum_vector(my_vector, num_arg, num_threads);

    timespec_get(&end_time, TIME_UTC);

    time_diff = calculate_runtime(start_time, end_time);
    runtime = to_second_float(time_diff);

    printf("Sum: %d\n", my_sum);
    printf("\n\nRuntime for core loop: %lf seconds.\n\n", runtime);

    free(my_vector);
    return 0;
}

int parallel_sum_vector(int vector[], int size, int num_threads)
{
    int total_sum = 0;
    int chunk_size = size / num_threads;
    int remainder = size % num_threads;
    
    int* partial_sums = malloc(num_threads * sizeof(int));
    for (int i = 0; i < num_threads; i++) {
        partial_sums[i] = 0;
    }
    
    for (int t = 0; t < num_threads; t++) {
        int start = t * chunk_size;
        int end = (t == num_threads - 1) ? (start + chunk_size + remainder) : (start + chunk_size);
        
        for (int i = start; i < end; i++) {
            partial_sums[t] += vector[i];
        }
    }
    
    for (int i = 0; i < num_threads; i++) {
        total_sum += partial_sums[i];
    }
    
    free(partial_sums);
    return total_sum;
}

void initialise_vector(int vector[], int size, int initial)
{
    for (int i = 0; i < size; i++)
    {
        vector[i] = initial;
    }
}

void non_trivial_vector(int vector[], int size)
{
    for (int i = 0; i < size; i++)
    {
        vector[i] = i + 2;
    }
}

double to_second_float(struct timespec in_time)
{
    return in_time.tv_sec + in_time.tv_nsec / 1e9;
}

struct timespec calculate_runtime(struct timespec start_time, struct timespec end_time)
{
    struct timespec time_diff;
    time_diff.tv_sec = end_time.tv_sec - start_time.tv_sec;
    time_diff.tv_nsec = end_time.tv_nsec - start_time.tv_nsec;
    if (time_diff.tv_nsec < 0)
    {
        time_diff.tv_sec -= 1;
        time_diff.tv_nsec += 1e9;
    }
    return time_diff;
}

int check_args(int argc, char **argv)
{
    if (argc == 2)
    {
        return atoi(argv[1]);
    }
    fprintf(stderr, "ERROR: You did not provide a numerical argument!\n");
    fprintf(stderr, "Correct use: %s [NUMBER]\n", argv[0]);
    exit(-1);
}
