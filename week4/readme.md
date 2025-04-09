# **Part 1**

*Step 1:*

When running this code for a low amount of processors, such as 2, it runs by sending a value to rank 0, which is then received from rank 1. The lines that receive the values are printed as follows "Hello, I am 0 of X".

This output is the same for any number of processors, with only the X value changing. The lines that have the "sent X to rank 0" output are also printed in a random order, rather than starting at 1 and ending at X

*Step 2:*
Updated code to be easier to read

*Step 3:*

So far it just runs and won't let me type anything else into the command line

*Step 4:*


# **Part 2**

*Step 1:*

Program created

*Step 2:*

Code initially was hanging after running but managed to get around this using online resources and help from software engineer.

With larger input values, the total time increases in an apparent linear fashion while the average time decreases.

The code lists all the times information was sent and received, as well as from what rank, like in the previous comm_test_mpi.c code

*Step 3:*

pingponga.c was created. This file takes two arguments in the command line. These arguments are the number of pings and the size of the array. This file only works with exaclty two processors to ensure it acts like "ping-pong". The root rank calls the main task to send an array to the receiving rank. The receiving rank modifes the array and returns it until the input number of pings value is reached

The main_task() function allocates the integer array of the same size of the input value for the size of the array

The graph part is incomplete

# **Part 3**

*Step 1:*

Prediction: MPI_Scatter will be the fastest as it only sends the necessary data.

Broadcast.c: initalises MPI and allocates vectors, as always. A non_trivial_vector is used so that summing the vector is meaningful. The root task has MPI_Bcast, which broadcasts the whole vector to all processes and waits for each client to send back a single integer. It then adds all received sums into a total and prints it.
