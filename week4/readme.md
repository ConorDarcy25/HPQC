# **Part 1**

*Step 1:*

When running this code for a low amount of processors, such as 2, it runs by sending a value to rank 0, which is then received from rank 1. The lines that receive the values are printed as follows "Hello, I am 0 of X".

This output is the same for any number of processors, with only the X value changing. The lines that have the "sent X to rank 0" output are also printed in a random order, rather than starting at 1 and ending at X

*Step 2:*
Updated code to be easier to read

*Step 3:*

Initally the code was hanging but I got it fixed. Send/Recv is better for simple message passing, while Scatter/Reduce is useful for real workloads as automatically balances workload and is fast.

*Step 4:*

Using 4 processors and a vector of 12, it took 0.413 seconds in real time.

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

The code takes one input - the vector size. For an input of 12 it took 0.000024 seconds.

Scatter.c: Same initialisation process as before. The vector is separated into chunks. For example is 12 is inputted and there are 4 processes, each process gets 3 values. Each process sums it's chunk and they are all gathered and summed using MPI_reducde. The result is then printed. Using the same amount of processors and same input value (4 and 12), it took 0.000044 seconds.

Send.c: Initialises as always. The root sends chuunks of the vector to other processes using MPI_Send. All process including the root compute a partial sum. Root gathers and sums all these using MPI_Reduce and prints the final sum. Takes one input. Using same values it took 0.000032 seconds.

According to my code, the broadcast code was the quickest.

*Step 2:*

Prediction: Gather.c

Gather.c: Rank 0 creates a vector and fill it with non trivial values. The vector is split using MPI_Scatter and the processors compute their own sums, like before. MPI_Gather is used to get all the partial sums, and rank 0 adds all them up and prints it. Using the same values it took 0.000021 seconds.

Reduce.c: Vector is initialised as before, and MPI_Scatter sends chunks to all processors as before. MPI_Reducde combines all the partial sums. Using the same values, MPI_Reduce took 0.000035 seconds.

While similar, MPI_Gather does not process (reduce) the data. Therefore MPI_Reduce is more efficient, generally, unless you want to just collect raw results. This makes my prediction correct, as there in one less process to go through.

*Step 3:*

This code initialises a vector of size n on rank 0. It splits it using MPI_Scatter and each process computes its local chunk. Two reductions are done using the built in MPI_Sum and custom reduction function. Using the same inputs as before, the predefined reduce function took 0.000131 seconds while the newly made function took 0.000002 seconds, meaning the homemade function was much quicker
