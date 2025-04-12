#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <mpi.h>


int check_args(int argc, char **argv);
void initialise_vector(double vector[], int size, double initial);
void print_vector(double vector[], int size);
int sum_vector(int vector[], int size);
void update_positions(double* pos, double* vel, double* acc, int n, double dt, double k, double m,
                      double left_neighbor, double right_neighbor, int is_first, int is_last);
int generate_timestamps(double* time_stamps, int time_steps, double step_size);
double driver(double time);
void print_header(FILE** p_out_file, int points);

int main(int argc, char **argv)
{
    int rank;
    int size;
    int i;

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    int points = check_args(argc, argv);

    int cycles = 5;
    int samples = 25;
    int time_steps = cycles * samples + 1;
    double step_size = 1.0 / samples;

    double dt = 0.01;
    double k = 1.0;
    double m = 1.0;

    double* time_stamps = (double*) malloc(time_steps * sizeof(double));
    for (i = 0; i < time_steps; i++) time_stamps[i] = 0.0;
    generate_timestamps(time_stamps, time_steps, step_size);

    int local_points = points / size;
    int remainder = points % size;
    if (rank < remainder) {
        local_points = local_points + 1;
    }

    double* local_pos = malloc(local_points * sizeof(double));
    double* local_vel = malloc(local_points * sizeof(double));
    double* local_acc = malloc(local_points * sizeof(double));
    for (i = 0; i < local_points; i++) {
        local_pos[i] = 0.0;
        local_vel[i] = 0.0;
        local_acc[i] = 0.0;
    }

    double* gathered_positions = NULL;
    int* recvcounts = NULL;
    int* displs = NULL;

    if (rank == 0) {
        gathered_positions = malloc(points * sizeof(double));
        recvcounts = malloc(size * sizeof(int));
        displs = malloc(size * sizeof(int));

        int total = 0;
        for (int p = 0; p < size; p++) {
            recvcounts[p] = points / size;
            if (p < remainder) {
                recvcounts[p]++;
            }
            displs[p] = total;
            total = total + recvcounts[p];
        }
    }

    FILE* out_file = NULL;
    if (rank == 0) {
        out_file = fopen("data/string_wave_parallel.csv", "w");
        if (out_file == NULL) {
            printf("Error opening file\n");
            MPI_Abort(MPI_COMM_WORLD, 1);
        }
        print_header(&out_file, points);
    }

    for (i = 0; i < time_steps; i++) {
        double current_time = time_stamps[i];

        double left_neighbor = 0.0;
        double right_neighbor = 0.0;
        double send_left = 0.0;
        double send_right = 0.0;

        if (local_points > 0) {
            send_left = local_pos[0];
            send_right = local_pos[local_points - 1];
        }

        if (rank > 0) {
            MPI_Sendrecv(&send_left, 1, MPI_DOUBLE, rank - 1, 0,
                         &left_neighbor, 1, MPI_DOUBLE, rank - 1, 1,
                         MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        }

        if (rank < size - 1) {
            MPI_Sendrecv(&send_right, 1, MPI_DOUBLE, rank + 1, 1,
                         &right_neighbor, 1, MPI_DOUBLE, rank + 1, 0,
                         MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        }

        int is_first = (rank == 0);
        int is_last = (rank == size - 1);

        update_positions(local_pos, local_vel, local_acc, local_points, dt, k, m,
                         left_neighbor, right_neighbor, is_first, is_last);

        MPI_Gatherv(local_pos, local_points, MPI_DOUBLE,
                    gathered_positions, recvcounts, displs, MPI_DOUBLE,
                    0, MPI_COMM_WORLD);

        if (rank == 0) {
            fprintf(out_file, "%d, %lf", i, current_time);
            for (int j = 0; j < points; j++) {
                fprintf(out_file, ", %lf", gathered_positions[j]);
            }
            fprintf(out_file, "\n");
        }
    }

    free(time_stamps);
    free(local_pos);
    free(local_vel);
    free(local_acc);

    if (rank == 0) {
        free(gathered_positions);
        free(recvcounts);
        free(displs);
        fclose(out_file);
    }

    MPI_Finalize();
    return 0;
}

void print_header(FILE** p_out_file, int points)
{
    int i;
    fprintf(*p_out_file, "#, time");
    for (i = 0; i < points; i++) {
        fprintf(*p_out_file, ", y[%d]", i);
    }
    fprintf(*p_out_file, "\n");
}

double driver(double time)
{
    double value = sin(time * 2.0 * M_PI);
    return value;
}

void update_positions(double* pos, double* vel, double* acc, int n, double dt, double k, double m,
                      double left_neighbor, double right_neighbor, int is_first, int is_last)
{
    for (int i = 0; i < n; i++) {
        double left = (i == 0) ? (is_first ? driver(0.0) : left_neighbor) : pos[i - 1];
        double right = (i == n - 1) ? (is_last ? 0.0 : right_neighbor) : pos[i + 1];

        acc[i] = (-k / m) * (2 * pos[i] - left - right);
    }

    for (int i = 0; i < n; i++) {
        vel[i] += acc[i] * dt;
        pos[i] += vel[i] * dt;
    }
}

int generate_timestamps(double* timestamps, int time_steps, double step_size)
{
    int i;
    for (i = 0; i < time_steps; i++) {
        timestamps[i] = i * step_size;
    }
    return time_steps;
}

int sum_vector(int vector[], int size)
{
    int sum = 0;
    for (int i = 0; i < size; i++) {
        sum = sum + vector[i];
    }
    return sum;
}

void initialise_vector(double vector[], int size, double initial)
{
    int i;
    for (i = 0; i < size; i++) {
        vector[i] = initial;
    }
}

void print_vector(double vector[], int size)
{
    int i;
    for (i = 0; i < size; i++) {
        printf("%d, %lf\n", i, vector[i]);
    }
}

int check_args(int argc, char **argv)
{
    int num_arg = 0;
    if (argc == 2) {
        num_arg = atoi(argv[1]);
    } else {
        fprintf(stderr, "ERROR: You did not provide a numerical argument!\n");
        fprintf(stderr, "Correct use: %s [NUMBER]\n", argv[0]);
        MPI_Abort(MPI_COMM_WORLD, -1);
    }
    return num_arg;
}
