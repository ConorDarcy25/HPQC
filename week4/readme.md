# **Part 1**

*Step 1:*

When running this code for a low amount of processors, such as 2, it runs by sending a value to rank 0, which is then received from rank 1. The lines that receive the values are printed as follows "Hello, I am 0 of X".

This output is the same for any number of processors, with only the X value changing. The lines that have the "sent X to rank 0" output are also printed in a random order, rather than starting at 1 and ending at X

*Step 2:*
Updated code to be easier to read
