import sys
import time; ms = time.time()*1000.0 

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
    start_time = time.time()
    # iterates over all numbers up to the input
    for i in range(in_arg):
        # prints the index
       # fp.write(str(print("{}, ".format(i), end="")))
        fp.write(str(i))
    # gets the end time for the loop
    end_time = time.time()

    # gets the total time
    run_time = end_time - start_time
    # prints the output

    #print("\n\nTime for loop: {} seconds\n".format(run_time))

   

    fp.write(str(run_time))
    fp.close


if __name__ == "__main__":
    main()

