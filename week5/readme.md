# **Part 1:**

*Step 2:*

Could not connect when trying to run python code

*Step 3:*

The code was modified to take 3 inputs from the command line. These inputs are the number of points along the string, the cycles and also the sample. 

The check_args() function checks the inputs and gives an error if they are incorrect. The time steps and step size are then calculated. Memory is then allocated. An output file is opened and the loop runs, updating the position each time with the positions being written to the csv file

Python code part is incomplete

# **Part 2:**

*Step 1:*

THe time_Steps function can be parallelised. Even though each time update depends on the previoius time step value, the loop can be parallelised to calculate different points in space while maintaining communication between segments

The string position calculation can be updated to be parallelised too by creating an array for each section of the string. Neighnouring chunks must still keep communication for boundary data.

The string will be divided into chunks with each chunk handling a set points, with the amount of points for each chunk depending on the amount of processes.

MPI_Send and MPI_Recv will be used to exchange boundary data between two segments as the process involves neighouring interactions

*Step 2:*

Each process will write it's output data to it's own assigned file

The root process will then gather all the output files and write them to a single csv file
