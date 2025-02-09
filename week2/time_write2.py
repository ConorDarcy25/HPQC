import sys
import time
from datetime import datetime

fp = open("data/time_py.txt", 'a')

def main(): 
    output = 0
    # checks if there are the right number of arguments
    try:
        # converts the first two arguments to integers
        in_arg = int(sys.argv[1])
    except: # (argc != 2)
        # raises an error
        raise Exception("Incorrect arguments.\nUsage: python time_print.py [NUM]\ne.g.\npython time_print.py 23")

    # gets the start time for the loop
    start_time = datetime.now()
    # iterates over all numbers up to the input
    for i in range(in_arg):
        # prints the index
       # fp.write(str(print("{}, ".format(i), end="")))
        fp.write(str(i))
    # gets the end time for the loop
    end_time = datetime.now()

    # gets the total time
    run_time = end_time - start_time
    # prints the output
  #  ms = run_time.total_seconds() * 1000
    ms = run_time.strftime(%S)
   

    fp.write("{ms}")
    fp.close


if __name__ == "__main__":
    main()
