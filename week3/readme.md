proof.c documentation:

main(): Starts the MPI environment, gets the ID, gets the total number of processes.

check functions: Checks if enough processes are available and determines what tasks shiould be done based on the ranks.

root_task(): Rank 0 is assigned as the root process, and waits to receive messages using MPI_Recv(), and ultimately sums all received values and prints the result

client_task(): Each process of rank > 0 is computed and the value is sent to the root process

check_args(): This piece of code ensures that the program uses one numerical argument as an integer

check_uni_size(): This code ensures that there are enough processes for the program to run

check_task(): This code states that root process calls root_task() while any other process calls client_task()

