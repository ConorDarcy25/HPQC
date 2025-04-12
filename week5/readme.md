# **Part 1:**

*Step 2:*

Could not connect when trying to run python code

*Step 3:*

The code was modified to take 3 inputs from the command line. These inputs are the number of points along the string, the cycles and also the sample. 

The check_args() function checks the inputs and gives an error if they are incorrect. The time steps and step size are then calculated. Memory is then allocated. An output file is opened and the loop runs, updating the position each time with the positions being written to the csv file

Python code part is incomplete

# **Part 2:**

*Step 1:*

The time_Steps function can be parallelised. Even though each time update depends on the previoius time step value, the loop can be parallelised to calculate different points in space while maintaining communication between segments

The string position calculation can be updated to be parallelised too by creating an array for each section of the string. Neighnouring chunks must still keep communication for boundary data.

The string will be divided into chunks with each chunk handling a set points, with the amount of points for each chunk depending on the amount of processes.

MPI_Send and MPI_Recv will be used to exchange boundary data between two segments as the process involves neighouring interactions.

*Step 2:*

To avoide multiple process conflicts form writin to the same file, MPI_Gather will be used. At each time step, each process will compute the positions for its given chunk of the string, adn will send the results of the calculation to rank 0. Rank 0 will then assemble the full string write to the output file.

*Step 3:*

This program initialises the MPI environment and give each process a rank, and gives the total amount of processes that are running. Check_Args is used to get the number of points from the command line argument. 5 cycles and 25 samples are used. Each step of the simulation is timed. The string is then divided as evenly as possible across all processes. Any extra points go to the lower numbered processes. Each process also has it's own array to store positions for its chunk. The root process then collects all the data using MPI_Gather and writes the combined data into a csv file.

The program takes one input (integer) from the command line

*Step 4:*

The program was tested using multiple values (100, 1000, 10000, 100000) and the execution time in both serial (-np 1) and parallel (-np >1) was measured. At lower values (100 and 1000) the serial code is as quicker, if not quicker, than the parallel code. However, at the higher input values scaling increases and communication time starts to matter.

It is worth parallelisnig but for medium to large simulations. For smaller simulations the parallelism adds overhead and provides little benefit. 

The solution is functional and writes to a CSV as expected, and the wave propagates as expected.

I couldn't get the python thing to work at all!

# **Part 3:**

Hooke's law was implemented in this code as the orignal code was not physically realistic. It is more realistic to treat each point on the string as a mass connected to its neighbours using springs.

The physical parameters were added (mass, spring constant) and new vectors were defined (position, velocity, acceleration). Euler integration was also used. MPI_Send and receive were used to exchange boundary values as each process had its own chunk of the string, which needed values from neighbouring ranks, to compute acceleration.

This new simulation created more realistic wave behaviour but requires more memory and computations per time step.
