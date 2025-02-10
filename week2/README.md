# Week 2

How to run:
time_write.py: This file is python code that takes an integer input, counts from 0 to whatever the input integer was, times how long it took for all that to occur, and then prints it all to a text file. The code was modified to not print to the command line.

time python3 HPQC/week2_tasks/time_write.py (integer)

time_write.c: This C file works like the python file, where a single integer value is inputted, counts up to that value and times how long it takes.

gcc HPQC/week2/time_write.c -o bin/time_write

time ./bin/time_write

time_read.py: This is a simple python file which takes the output text file from the “time_write.py” file, times how long it takes to read the text file and prints that time to the command line.

time_read.c: This C file works the same as the python equivalent also, where it reads a file as input and times how long for it to be read.

gcc HPQC/week2/time_read.c -o bin/time_read

time ./bin/time_read


Conclusions:
hello_world.py/hello_world.c: Both of these programs ran extremely quickly, but when comparing the two it’s clear that the C code is much more efficient as the Python code had a real runtime of 0.018s and the C code had a real runtime of 0.005s

repeat_adder.py/repeat_adder.c: When taking small inputs, in this case 4 and 6, the C code ran much quicker than the python code, with real run times of 0.004s and 0.024s respectively. When using slightly larger values such as 1000 and 2000, the same result was observed. However, when using two extremely large values, both being 123456789, the python code took a significant amount of time to give the correct answer, taking 11.424s, while the C code took 0.347s to output an incorrect value of -1757895751. The C code was run twice to ensure that it was not a human/input error, with the answer ultimately being the same showing that the python code is more efficient in this regard.

time_write/time_read programs: These files (both the python and the C files) were ran for various inputs. It was found that like in the hello_world files, C was superior when it came to run time, with the C code consistently taking about ¼ of the time it took the Python files to run. However, there was a lot of difficulty when it came to editing the C files. The C files were much more complicated to edit, especially for one with no experience with C. Both C files had ~100 lines of code, while the Python codes had less than half of that to give the exact same result, albeit with slightly slower run times

