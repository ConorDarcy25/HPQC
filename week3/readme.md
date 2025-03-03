Part 1:
Benchmarking:

When running the file for low values (for example, 4 and 10) the real times are similar. These times were approximately 0.5s with the higher input value even having a slightly shorter real run time (0.417s vs 0.552s). Each real time was larger than their respective user and system times. For lower values, the times were similar to that of the serial code. 

When inputting a very large value, such as 100, the code would not run as there was not enough slots available in the code to compute such a large input value


Part 2:
proof.c documentation:

main(): Starts the MPI environment, gets the ID, gets the total number of processes.

check functions: Checks if enough processes are available and determines what tasks shiould be done based on the ranks.

root_task(): Rank 0 is assigned as the root process, and waits to receive messages using MPI_Recv(), and ultimately sums all received values and prints the result

client_task(): Each process of rank > 0 is computed and the value is sent to the root process

check_args(): This piece of code ensures that the program uses one numerical argument as an integer

check_uni_size(): This code ensures that there are enough processes for the program to run

check_task(): This code states that root process calls root_task() while any other process calls client_task()


Part 3:

Understanding of the serial code:

At the start of the code, functions are delcared that will be used in main()

In the main() function, check_args checks the program to make sure only ine argument is given. If this is not the case, an errir message is returned. Malloc is used to dynamically alocate memory for the vector. The vector is then initialised to set each element of the vector to 0. The vector is then freed

The check_args function checks if the user gives only one argument, and converts the integer value using the "atoi" function

The initialise function initialises all elements in a vector to the given value.

The sum of vectors is then calculated and printed


Non-trivialisation:

The <time.h> library has been added to the code which allows one to measure the runtime of the procedure. This is done by starting a timer before the vector addition and then finishing it once it's completed. The initailisation function was modified to give a vector value of i + 2, or num_arg + 1.

Parallelisation: 

While the serial code 
