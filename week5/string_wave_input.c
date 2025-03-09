#include <stdio.h>
#include <stdlib.h>
#include <math.h>

// Global variables instead of struct
int points;
int cycles;
int samples;

void initialise_vector(double vector[], int size, double initial);
void print_header(FILE* p_out_file, int points);
void update_positions(double* positions, int points, double time);
int generate_timestamps(double* time_stamps, int time_steps, double step_size);
double driver(double time);

int main(int argc, char **argv) {
    // Basic argument parsing without struct
    if (argc != 4) {
        printf("Usage: %s points cycles samples\n", argv[0]);
        return 1;
    }
    points = atoi(argv[1]);
    cycles = atoi(argv[2]);
    samples = atoi(argv[3]);

    int time_steps = cycles * samples + 1;
    double step_size = 1.0 / samples;

    double* time_stamps = malloc(time_steps * sizeof(double));
    if (time_stamps == NULL) {
        printf("Memory allocation failed for time_stamps\n");
        return 1;
    }
    initialise_vector(time_stamps, time_steps, 0.0);
    generate_timestamps(time_stamps, time_steps, step_size);

    double* positions = malloc(points * sizeof(double));
    if (positions == NULL) {
        printf("Memory allocation failed for positions\n");
        free(time_stamps);
        return 1;
    }
    initialise_vector(positions, points, 0.0);

    FILE* out_file = fopen("string_wave.csv", "w");
    if (out_file == NULL) {
        printf("Error opening file\n");
        free(time_stamps);
        free(positions);
        return 1;
    }
    print_header(out_file, points);

    for (int i = 0; i < time_steps; i++) {
        update_positions(positions, points, time_stamps[i]);
        fprintf(out_file, "%d, %lf", i, time_stamps[i]);
        for (int j = 0; j < points; j++) {
            fprintf(out_file, ", %lf", positions[j]);
        }
        fprintf(out_file, "\n");
    }

    free(time_stamps);
    free(positions);
    fclose(out_file);
    return 0;
}

void print_header(FILE* p_out_file, int points) {
    fprintf(p_out_file, "#, time");
    for (int j = 0; j < points; j++) {
        fprintf(p_out_file, ", y[%d]", j);
    }
    fprintf(p_out_file, "\n");
}

double driver(double time) {
    return sin(time * 2.0 * M_PI);
}

void update_positions(double* positions, int points, double time) {
    double* new_positions = malloc(points * sizeof(double));
    if (new_positions == NULL) {
        printf("Memory allocation failed for new_positions\n");
        return;
    }
    new_positions[0] = driver(time);
    for (int i = 1; i < points; i++) {
        new_positions[i] = positions[i - 1];
    }
    for (int i = 0; i < points; i++) {
        positions[i] = new_positions[i];
    }
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
