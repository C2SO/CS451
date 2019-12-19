Nicholas Rahbany
CS451
README.txt

I HAVE USED ONE COUPON ON THIS ASSIGNMENT

To compile the program, navigate to the directory in the
terminal and type the command: 'gcc -o A4.o A4.c'

To run the program, type the command: "./A4.o {memory_size}'
where 'memory_size' will be the amount of memory you want the
program to "allocate".

This program has five different functions:
'RQ process_number memory_size fit' - Request memory
    process_number - An integer passed in that will be used to
                     differentiate one process from another
    memory_size - An integer passed in that will be used to tell
                  the program how much memory you want the
                  program to have allocated toward it
    fit - The fit algorithm that will be used to determine where
          the process will be placed in memory. There are three
          differet kinds of fit:
              F: First Fit - Will place the process in the First
                             available slot of memory that can
                             hold it
              W: Worst Fit - Will place the process in the
                             largest slot of free space that can
                             hold it
              B: Best Fit - Will place the process in the
                            smallest slot of free space that can
                            hold it
'RL process_number' - Release memory
    process_number - An integer passed in that will be used to
                     differentiate one process from another
'C' - Compress memory
'STAT' - Prints out the current memory
'X' - Exit program