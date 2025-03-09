#include <stdio.h>
#include <stdlib.h>
#include <math.h>

// declares the functions that will be called within main
// note how declaration lines are similar to the initial line
// of a function definition, but with a semicolon at the end;
int check_args(int argc, char **argv, int* points, int* cycles, int* samples);
void initialise_vector(double vector[], int size, double initial);
void print_vector(double vector[], int size);
int sum_vector(int vector[], int size);
void update_positions(double* positions, int points, double time);
int generate_timestamps(double* time_stamps, int time_steps, double step_size);
double driver(double time);
void print_header(FILE** p_out_file, int points);

int main(int argc, char **argv)
{
    // declare and initialise the numerical argument variables
    int points, cycles, samples;
    if (check_args(argc, argv, &points, &cycles, &samples) != 0) {
        return 1;
    }

    // calculates time steps
    int time_steps = cycles * samples + 1;
    double step_size = 1.0 / samples;

    // creates a vector for the time stamps in the data
    double* time_stamps = (double*) malloc(time_steps * sizeof(double));
    if (time_stamps == NULL) {
        printf("Memory allocation failed for time_stamps\n");
        return 1;
    }
    initialise_vector(time_stamps, time_steps, 0.0);
    generate_timestamps(time_stamps, time_steps, step_size);

    // creates a vector variable for the current positions
    double* positions = (double*) malloc(points * sizeof(double));
    if (positions == NULL) {
        printf("Memory allocation failed for positions\n");
        free(time_stamps);
        return 1;
    }
    initialise_vector(positions, points, 0.0);

    // creates a file
    FILE* out_file;
    out_file = fopen("string_wave.csv", "w");
    if (out_file == NULL) {
        printf("Error opening file\n");
        free(time_stamps);
        free(positions);
        return 1;
    }
    print_header(&out_file, points);

    // iterates through each time step in the collection
    for (int i = 0; i < time_steps; i++)
    {
        // updates the position using a function
        update_positions(positions, points, time_stamps[i]);

        // prints an index and time stamp
        fprintf(out_file, "%d, %lf", i, time_stamps[i]);

        // iterates over all of the points on the line
        for (int j = 0; j < points; j++)
        {
            // prints each y-position to a file
            fprintf(out_file, ", %lf", positions[j]);
        }
        // prints a new line
        fprintf(out_file, "\n");
    }

    // if we use malloc, must free when done!
    free(time_stamps);
    free(positions);

    // closes the file
    fclose(out_file);

    return 0;
}

// defines a function that checks your arguments to make sure they'll do what you need
int check_args(int argc, char **argv, int* points, int* cycles, int* samples)
{
    if (argc != 4) {
        printf("Usage: %s points cycles samples\n", argv[0]);
        return 1;
    }
    *points = atoi(argv[1]);
    *cycles = atoi(argv[2]);
    *samples = atoi(argv[3]);
    return 0;
}

// prints a header to the file
// double-pointer used to allow this function to move the file pointer
void print_header(FILE** p_out_file, int points)
{
    fprintf(*p_out_file, "#, time");
    for (int j = 0; j < points; j++)
    {
        fprintf(*p_out_file, ", y[%d]", j);
    }
    fprintf(*p_out_file, "\n");
}

// defines a simple harmonic oscillator as the driving force
double driver(double time)
{
    return sin(time * 2.0 * M_PI);
}

// defines a function to update the positions
void update_positions(double* positions, int points, double time)
{
    double* new_positions = (double*) malloc(points * sizeof(double));
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

// defines a set of timestamps
int generate_timestamps(double* timestamps, int time_steps, double step_size)
{
    for (int i = 0; i < time_steps ; i++)
    {
        timestamps[i] = i * step_size;
    }    
    return time_steps;
}

// defines a function to initialise all values in a vector to a given initial value
void initialise_vector(double vector[], int size, double initial)
{
    for (int i = 0; i < size; i++)
    {
        vector[i] = initial;
    }
}
