#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <mpi.h>

// Declare functions
int check_args(int argc, char **argv);
void initialise_vector(double vector[], int size, double initial);
void print_vector(double vector[], int size);
int sum_vector(int vector[], int size);
void update_positions(double* positions, int points, double time);
int generate_timestamps(double* time_stamps, int time_steps, double step_size);
double driver(double time);
void print_header(FILE** p_out_file, int points);

int main(int argc, char **argv)
{
    // Initialize MPI
    MPI_Init(&argc, &argv);

    int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    int points = check_args(argc, argv);

    int cycles = 5;
    int samples = 25;
    int time_steps = cycles * samples + 1;
    double step_size = 1.0 / samples;

    // Allocate memory for time stamps
    double* time_stamps = (double*) malloc(time_steps * sizeof(double));
    if (time_stamps == NULL) {
        fprintf(stderr, "ERROR: Memory allocation failed for time_stamps\n");
        MPI_Finalize();
        return -1;
    }
    initialise_vector(time_stamps, time_steps, 0.0);
    generate_timestamps(time_stamps, time_steps, step_size);

    // Divide work among processes
    int chunk_size = points / size;
    int start = rank * chunk_size;
    int end = start + chunk_size;
    if (rank == size - 1) {
        end = points;
    }

    // Allocate memory for positions
    double* positions = (double*) malloc(points * sizeof(double));
    if (positions == NULL) {
        fprintf(stderr, "ERROR: Memory allocation failed for positions\n");
        free(time_stamps);
        MPI_Finalize();
        return -1;
    }
    initialise_vector(positions, points, 0.0);

    // Each process writes its own file
    char filename[50];
    sprintf(filename, "output_rank_%d.csv", rank);
    FILE* out_file = fopen(filename, "w");
    if (out_file == NULL) {
        fprintf(stderr, "ERROR: Failed to open file %s\n", filename);
        free(time_stamps);
        free(positions);
        MPI_Finalize();
        return -1;
    }

    if (rank == 0) {
        print_header(&out_file, points);
    }

    for (int i = 0; i < time_steps; i++)
    {
        update_positions(positions, points, time_stamps[i]);

        fprintf(out_file, "%d, %lf", i, time_stamps[i]);
        for (int j = start; j < end; j++)
        {
            fprintf(out_file, ", %lf", positions[j]);
        }
        fprintf(out_file, "\n");
    }

    fclose(out_file);
    free(time_stamps);
    free(positions);

    // Root process collects results
    if (rank == 0) {
        FILE* final_out_file = fopen("final_output.csv", "w");
        if (final_out_file == NULL) {
            fprintf(stderr, "ERROR: Failed to create final output file\n");
            MPI_Finalize();
            return -1;
        }
        print_header(&final_out_file, points);

        for (int i = 0; i < size; i++) {
            char temp_filename[50];
            sprintf(temp_filename, "output_rank_%d.csv", i);

            FILE* temp_file = fopen(temp_filename, "r");
            if (temp_file == NULL) {
                fprintf(stderr, "WARNING: Could not open file %s\n", temp_filename);
                continue;
            }

            char line[1024];
            fgets(line, sizeof(line), temp_file);

            while (fgets(line, sizeof(line), temp_file)) {
                fprintf(final_out_file, "%s", line);
            }
            fclose(temp_file);
        }
        fclose(final_out_file);
    }

    MPI_Finalize();
    return 0;
}

// Function definitions
void print_header(FILE** p_out_file, int points)
{
    fprintf(*p_out_file, "#, time");
    for (int j = 0; j < points; j++)
    {
        fprintf(*p_out_file, ", y[%d]", j);
    }
    fprintf(*p_out_file, "\n");
}

double driver(double time)
{
    double value = sin(time * 2.0 * M_PI);
    return value;
}

void update_positions(double* positions, int points, double time)
{
    double* new_positions = (double*) malloc(points * sizeof(double));
    if (new_positions == NULL) {
        fprintf(stderr, "ERROR: Memory allocation failed for new_positions\n");
        return;
    }

    int i = 0;
    new_positions[i] = driver(time);
    for (i = 1; i < points; i++)
    {
        new_positions[i] = positions[i - 1];
    }
    for (i = 0; i < points; i++)
    {
        positions[i] = new_positions[i];
    }
    free(new_positions);
}

int generate_timestamps(double* timestamps, int time_steps, double step_size)
{
    for (int i = 0; i < time_steps; i++)
    {
        timestamps[i] = i * step_size;
    }
    return time_steps;
}

void initialise_vector(double vector[], int size, double initial)
{
    for (int i = 0; i < size; i++)
    {
        vector[i] = initial;
    }
}

int check_args(int argc, char **argv)
{
    int num_arg = 0;

    if (argc == 2)
    {
        num_arg = atoi(argv[1]);
    }
    else
    {
        fprintf(stderr, "ERROR: You did not provide a numerical argument!\n");
        fprintf(stderr, "Correct use: %s [NUMBER]\n", argv[0]);
        exit(-1);
    }
    return num_arg;
}

