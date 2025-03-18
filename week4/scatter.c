#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <mpi.h>

void initialise_vector(int vector[], int size, int initial);
void non_trivial_vector(int vector[], int size);
int sum_vector(int vector[], int size, int start);
void print_vector(int vector[], int size);

double to_second_float(struct timespec in_time);
struct timespec calculate_runtime(struct timespec start_time, struct timespec end_time);

int main(int argc, char **argv) {
    int rank, size, n;
    struct timespec start_time, end_time, time_diff;
    double runtime = 0.0;
    
    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    
    if (argc != 2) {
        if (rank == 0) {
            fprintf(stderr, "Usage: %s [VECTOR_SIZE]\n", argv[0]);
        }
        MPI_Finalize();
        exit(-1);
    }
    
    n = atoi(argv[1]);
    int local_size = n / size;
    int *vector = NULL;
    int *local_vector = (int*)malloc(local_size * sizeof(int));
    
    if (rank == 0) {
        vector = (int*)malloc(n * sizeof(int));
        initialise_vector(vector, n, 0);
        non_trivial_vector(vector, n);
    }
    
    timespec_get(&start_time, TIME_UTC);
    MPI_Scatter(vector, local_size, MPI_INT, local_vector, local_size, MPI_INT, 0, MPI_COMM_WORLD);
    timespec_get(&end_time, TIME_UTC);
    
    time_diff = calculate_runtime(start_time, end_time);
    runtime = to_second_float(time_diff);
    
    if (rank == 0) {
        printf("MPI_Scatter time: %lf seconds\n", runtime);
    }
    
    int partial_sum = sum_vector(local_vector, local_size, 0);
    int total_sum;
    
    MPI_Reduce(&partial_sum, &total_sum, 1, MPI_INT, MPI_SUM, 0, MPI_COMM_WORLD);
    
    if (rank == 0) {
        printf("Global sum: %d\n", total_sum);
        free(vector);
    }
    
    free(local_vector);
    MPI_Finalize();
    return 0;
}

void initialise_vector(int vector[], int size, int initial) {
    for (int i = 0; i < size; i++) {
        vector[i] = initial;
    }
}

void non_trivial_vector(int vector[], int size) {
    for (int i = 0; i < size; i++) {
        vector[i] = i + 2;
    }
}

int sum_vector(int vector[], int size, int start) {
    int sum = 0;
    for (int i = 0; i < size; i++) {
        sum += vector[start + i];
    }
    return sum;
}

void print_vector(int vector[], int size) {
    for (int i = 0; i < size; i++) {
        printf("%d\n", vector[i]);
    }
}

double to_second_float(struct timespec in_time) {
    return in_time.tv_sec + in_time.tv_nsec / 1e9;
}

struct timespec calculate_runtime(struct timespec start_time, struct timespec end_time) {
    struct timespec time_diff;
    time_diff.tv_sec = end_time.tv_sec - start_time.tv_sec;
    time_diff.tv_nsec = end_time.tv_nsec - start_time.tv_nsec;
    if (time_diff.tv_nsec < 0) {
        time_diff.tv_sec -= 1;
        time_diff.tv_nsec += 1e9;
    }
    return time_diff;
}
