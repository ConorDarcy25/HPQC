#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>

// Struct to store command-line arguments
typedef struct {
    int points;
    int cycles;
    int samples;
    char output_file[256];
} Args;

int check_args(int argc, char **argv, Args *args);
void initialise_vector(double vector[], int size, double initial);
void print_header(FILE** p_out_file, int points);
void update_positions(double* positions, int points, double time);
int generate_timestamps(double* time_stamps, int time_steps, double step_size);
double driver(double time);

int main(int argc, char **argv) {
    // Parse arguments
    Args args;
    check_args(argc, argv, &args);

    int time_steps = args.cycles * args.samples + 1;
    double step_size = 1.0 / args.samples;

    // Allocate memory for timestamps and positions
    double* time_stamps = (double*) malloc(time_steps * sizeof(double));
    initialise_vector(time_stamps, time_steps, 0.0);
    generate_timestamps(time_stamps, time_steps, step_size);

    double* positions = (double*) malloc(args.points * sizeof(double));
    initialise_vector(positions, args.points, 0.0);

    // Open file for writing
    FILE* out_file = fopen(args.output_file, "w");
    if (!out_file) {
        fprintf(stderr, "ERROR: Unable to open file %s for writing.\n", args.output_file);
        exit(EXIT_FAILURE);
    }
    print_header(&out_file, args.points);

    for (int i = 0; i < time_steps; i++) {
        update_positions(positions, args.points, time_stamps[i]);
        fprintf(out_file, "%d, %lf", i, time_stamps[i]);
        for (int j = 0; j < args.points; j++) {
            fprintf(out_file, ", %lf", positions[j]);
        }
        fprintf(out_file, "\n");
    }

    free(time_stamps);
    free(positions);
    fclose(out_file);
    return 0;
}

int check_args(int argc, char **argv, Args *args) {
    if (argc != 5) {
        fprintf(stderr, "Usage: %s points cycles samples output_file\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    args->points = atoi(argv[1]);
    args->cycles = atoi(argv[2]);
    args->samples = atoi(argv[3]);
    strncpy(args->output_file, argv[4], 255);
    args->output_file[255] = '\0';
    return 0;
}

void print_header(FILE** p_out_file, int points) {
    fprintf(*p_out_file, "#, time");
    for (int j = 0; j < points; j++) {
        fprintf(*p_out_file, ", y[%d]", j);
    }
    fprintf(*p_out_file, "\n");
}

double driver(double time) {
    return sin(time * 2.0 * M_PI);
}

void update_positions(double* positions, int points, double time) {
    double* new_positions = (double*) malloc(points * sizeof(double));
    new_positions[0] = driver(time);
    for (int i = 1; i < points; i++) {
        new_positions[i] = positions[i - 1];
    }
    memcpy(positions, new_positions, points * sizeof(double));
    free(new_positions);
}

int generate_timestamps(double* timestamps, int time_steps, double step_size) {
    for (int i = 0; i < time_steps; i++) {
        timestamps[i] = i * step_size;
    }
    return time_steps;
}

void initialise_vector(double vector[], int size, double initial) {
    for (int i = 0; i < size; i++) {
        vector[i] = initial;
    }
}
