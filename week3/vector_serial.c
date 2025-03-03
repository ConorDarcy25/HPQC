#include <stdio.h>
#include <stdlib.h>

int check_args(int argc, char **argv);
void initialise_vector(int vector[], int size, int initial);
void print_vector(int vector[], int size);
int sum_vector(int vector[], int size);
void non_trivial_vector(int vector[], int size);


int main(int argc, char **argv)
{
	
	int num_arg = check_args(argc, argv);

	int* my_vector = malloc (num_arg * sizeof(int));
	
	initialise_vector(my_vector, num_arg, 0);


	
	non_trivial_vector(my_vector, num_arg);



	int my_sum = sum_vector(my_vector, num_arg);


	printf("Sum: %d\n", my_sum);
	
	free(my_vector);

	return 0;
}


int sum_vector(int vector[], int size)
{
	int sum = 0;

	
	for (int i = 0; i < size; i++)
	{
		sum += vector[i];
	}

	
	return sum;
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
		
		vector[i] = i+2;
	}
}



void print_vector(int vector[], int size)
{
	
	for (int i = 0; i < size; i++)
	{
		
		printf("%d\n", vector[i]);
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

		
		exit (-1);
	}
	return num_arg;
}
