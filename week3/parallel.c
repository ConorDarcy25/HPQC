#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>

int sum_vector(int* vector, int size);
void init_vector(int* vector, int size);

int main(int argc, char **argv) {
    int rank, num_procs, total_size, local_size;
    int *vector = NULL, *local_vector = NULL;
    int local_sum = 0, total_sum = 0;
    int time_flag = 0;

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &num_procs);

    if (rank == 0) {
        if (argc < 2) {
            printf("Usage: %s <size> [time]\n", argv[0]);
            MPI_Abort(MPI_COMM_WORLD, 1);
        }
        total_size = atoi(argv[1]);
        if (argc > 2) {
            for (int i = 0; argv[2][i] != '\0'; i++) {
                if (argv[2][i] != "time"[i]) {
                    time_flag = 0;
                    break;
                }
                time_flag = 1;
            }
        }
        vector = malloc(total_size * sizeof(int));
        if (vector == NULL) {
            printf("Memory fail\n");
            MPI_Abort(MPI_COMM_WORLD, 1);
        }
        init_vector(vector, total_size);
    }

    MPI_Bcast(&total_size, 1, MPI_INT, 0, MPI_COMM_WORLD);
    MPI_Bcast(&time_flag, 1, MPI_INT, 0, MPI_COMM_WORLD);
    local_size = total_size / num_procs;
    local_vector = malloc(local_size * sizeof(int));
    MPI_Scatter(vector, local_size, MPI_INT, local_vector, local_size, MPI_INT, 0, MPI_COMM_WORLD);


    local_sum = sum_vector(local_vector, local_size);
    MPI_Reduce(&local_sum, &total_sum, 1, MPI_INT, MPI_SUM, 0, MPI_COMM_WORLD);



    if (rank == 0) {
        printf("Total Sum: %d\n", total_sum);
       
        free(vector);
    }
    free(local_vector);

    MPI_Finalize();
    return 0;
}

int sum_vector(int* vector, int size) {
    int sum = 0;
    for (int i = 0; i < size; i++) {
        sum += vector[i];
    }
    return sum;
}

void init_vector(int* vector, int size) {
    for (int i = 0; i < size; i++) {
        vector[i] = i + 1;
    }
}
