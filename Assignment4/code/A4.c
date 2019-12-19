// Nicholas Rahbany
// CS 451
// A4.c

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int memalloc; // A global variable that refferences how much
              // memory a user requested

// worst_fit
// Inputs:
//     projectMemory - The "data" that the program will be
//                     manipulating
//     process - The process number that the user entered
//     size - The ammount of memory the user wants allocates
//            to the specified process
// Function:
//     Uses the "Worst Fit" algorithm to allocate memory for
//         a process
void worst_fit(int *projectMemory, int process, int size)
{
    printf("Adding %d bytes with Worst Fit...\n", size);

    int maxSizeStartFound = -1; // Used as the index for
                                // allocating memory
    int maxSizeFound = 0;       // Used to compare which block
                                // of "free space" is the
                                // largest

    for (int i = 0; i < memalloc; i++)
    // For all the bytes in memory
    {
        if (projectMemory[i] == -1)
        // If the current space is free
        {
            int maxSizeChecker = 1; // Number of free spaces in
                                    // a row
            int j = i + 1;          // Index for next loop
            while (projectMemory[j] == -1 && j < memalloc)
            // While the current space is free and the index
            // is less than the memory allocated
            {
                maxSizeChecker++; // Increase the size of
                                  // maxSizeChecker
                j++;              // Increment the current
                                  // index
            }

            if (maxSizeChecker > maxSizeFound)
            // If the most recently checked block of free space
            // is greater than the previously checked
            {
                maxSizeFound = maxSizeChecker; // Update
                                               // maxSizeFound
                maxSizeStartFound = i;         // Set the new
                                               // index to the
                                               // current index
                i += maxSizeFound;             // Save time by
                                               // Moving to the
                                               // next index
            }
        }
    }

    if (maxSizeFound >= size)
    // If maxSizeFound can fit the processes requested size
    {
        for (int i = maxSizeStartFound;
             i < size + maxSizeStartFound;
             i++)
            // For each byte the process requres
            projectMemory[i] = process; // Allocate the block to
                                        // the specified process
        printf(
            "Process %d successfully requested %d bytes.\n",
            process, size);
    }
    else
        printf("There was not enough space to add the process.\n");
}

// best_fit
// Inputs:
//     projectMemory - The "data" that the program will be
//                     manipulating
//     process - The process number that the user entered
//     size - The ammount of memory the user wants allocates
//            to the specified process
// Function:
//     Uses the "Best Fit" algorithm to allocate memory for
//         a process
void best_fit(int *projectMemory, int process, int size)
{
    printf("Adding %d bytes with Best Fit...\n", size);

    int minSizeStartFound = -1; // The index for the smallest
                                // block that can fit the
                                // process
    int minSizeFound = 0;       // The size of the smallest block

    for (int i = 0; i < memalloc; i++)
    // For all the bytes in memory
    {
        if (projectMemory[i] == -1)
        // If the current slot is free
        {
            int minSizeChecker = 1; // Number of free spaces in
                                    // a row
            int j = i + 1;          // Index used for next loop

            while (projectMemory[j] == -1 && j < memalloc)
            // While the current space is free and the index is
            // less than the memory allocated
            {
                minSizeChecker++; // Increment minSizeChecker
                j++;              // increment the index
            }

            if ((minSizeChecker < minSizeFound ||
                 minSizeFound == 0) &&
                minSizeChecker >= size)
            // If the new minimum is less than the previous
            // or if there is no previous space checked
            // and the space can fit the process
            {
                minSizeFound = minSizeChecker; // Update
                                               // minSizeFound
                minSizeStartFound = i;         // Update
                                               // minSizeStartFound
                i += minSizeFound;             // Update the index to save
                                               // time
            }
        }
    }

    if (minSizeFound >= size)
    // If the smallest gap can hold the size the process
    // requested
    {
        for (int j = minSizeStartFound;
             j < size + minSizeStartFound;
             j++)
            // For each byte in the process
            projectMemory[j] = process; // Allocate the memory
        printf("Process %d successfully requested %d bytes.\n",
               process, size);
    }
    else
        printf("There was not enough space to add the process.\n");
}

// first_fit
// Inputs:
//     projectMemory - The "data" that the program will be
//                     manipulating
//     process - The process number that the user entered
//     size - The ammount of memory the user wants allocates
//            to the specified process
// Function:
//     Uses the "First Fit" algorithm to allocate memory for
//         a process
void first_fit(int *projectMemory, int process, int size)
{
    printf("Adding %d bytes with First Fit...\n", size);

    int added = -1; // Boolean used to check if the process
                    // has been added to memory

    for (int i = 0; i < memalloc; i++)
    // For all the bytes in memory
    {
        if (added == -1)
        // If the process is not added
        {
            int freeMemory = -1; // Used to determine if the
                                 // memory is free
            if (projectMemory[i] == -1)
            // If the current slot is free
            {
                int freeCount = 1; // Used to keep track of the
                                   // number of free spaces in
                                   // a row
                for (int j = i + 1; j < i + size; j++)
                // For the bytes following the current index
                {
                    if (projectMemory[j] == -1)
                        // If the current slot is free
                        freeCount++; // Increment freeCount
                }

                if (freeCount == size)
                // If there are enough spaces in a row to fit
                // the process
                {
                    for (int j = 0; j < size; j++)
                        // For each byte the process needs
                        projectMemory[i + j] = process;
                    // Allocate the memory
                    added = 1; // Change the boolean to tell the
                               // function the process has been
                               // added
                }
                else
                    i = i + freeCount - 1; // Increment the index
                                           // to save time
            }
            if (i == memalloc - 1)
            // If the process has not been added
            {
                printf("There was not enough space");
                printf(" to add the process.\n");
            }

            if (added == 1)
            // If the process was added
            {
                printf("Process successfully requested %d bytes.\n",
                       size);
            }
        }
    }
}

// request
// Inputs:
//     projectMemory - The "data" that the program will be
//                     manipulating
//     process - The process number that the user entered
//     size - The ammount of memory the user wants allocates
//            to the specified process
//     fit - The determined algorithm to fit the process in
//           memory
// Function:
//     Calls the necessary function based on what the user
//     requests in "fit"
void request(int *projectMemory, int process, int size, char *fit)
{
    if (strcmp(fit, "W") == 0)
        // If the user requested the "W" fit algorithm
        worst_fit(projectMemory, process, size);
    else if (strcmp(fit, "B") == 0)
        // If the user requested the "B" fit algorithm
        best_fit(projectMemory, process, size);
    else if (strcmp(fit, "F") == 0)
        // If the user requested the "F" fit algorithm
        first_fit(projectMemory, process, size);
    else
        // If the user input is invalid
        printf("No valid fit was found in the input.\n");
}

// release
// Inputs:
//     projectMemory - The "data" that the program will be
//                     manipulating
//     process - The process number that the user entered
// Function:
//     Released the memory allocated by a process
void release(int *projectMemory, int process)
{
    for (int i = 0; i < memalloc; i++)
    // For each slot in memory
    {
        if (projectMemory[i] == process)
        // If the current slot is occupied by the specified
        // process
        {
            projectMemory[i] = -1; // Free the slot
        }
    }
}

// release
// Inputs:
//     projectMemory - The "data" that the program will be
//                     manipulating
// Function:
//     Prints out the current memory allocation
void statistics(int *projectMemory)
{
    printf("Addresses [0:");
    int currSlot = projectMemory[0]; // Define the first slot as
                                     // the base comparrison

    for (int i = 1; i < memalloc; i++)
    // For each slot in memory
    {
        if (projectMemory[i] != currSlot && i != memalloc - 1)
        // If the current slot does not match the previous slots
        // and the loop has not reached the last slot
        {
            if (currSlot == -1)
                // If the current slot is empty
                printf("%d] Unused\n", i - 1);

            else
                // If the current slot is occupied
                printf("%d] Process P%d\n", i - 1, currSlot);

            printf("Addresses [%d:", i);
            currSlot = projectMemory[i]; // Update currSlot
        }

        else if (i == memalloc - 1)
        // If the index is at the last slot
        {
            if (currSlot == -1)
                // If the block of memory is unused
                printf("%d] Unused\n", i);

            else
                // If the block of memory is used
                printf("%d] Process P%d\n", i, currSlot);
        }
    }
}

// compress
// Inputs:
//     projectMemory - The "data" that the program will be
//                     manipulating
// Function:
//     Organizes the memory to make all allocated memory
//     next to each other
void compress(int *projectMemory)
{
    int none = -1; // index used to define free spaces
    int index = 0; // Index used to determine the index of
                   // the first free space

    while (none == -1 && index < memalloc)
    // While none is undefined and the index is less than the
    // memory available
    {
        if (projectMemory[index] == -1)
            // If the current slot is free
            none = index; // Update none to hold the current
                          // index
        index++;          // Increment index
    }

    for (int i = none + 1; i < memalloc; i++)
    // For all unchecked slots in memory
    {
        if (projectMemory[i] == -1 && projectMemory[none] != -1)
            // If the current slot is free and the slot at index
            // "none" is occupied
            none = i; // Update none to hold the current index
        else if (projectMemory[i] != -1 &&
                 projectMemory[none] == -1)
        // Else, if the current slot is occupied and the slot
        // at index "none" is free
        {
            projectMemory[none] = projectMemory[i]; // Update
                                                    // the free
                                                    // slot
            projectMemory[i] = -1;                  // Set the
                                                    // current
                                                    // slot to
                                                    // free
            none++;                                 // Increment
                                                    // none
        }
    }
}

// compress
// Inputs:
//     userResponse - The input the user passes into the console
//     projectMemory - the "data" that the program will be
//                     manipulating
// Function:
//     Takes in the user's command and passes it off
//     to the proper function
void parse_args(char *userResponse, int *projectMemory)
{
    int len = strlen(userResponse); // Get the length of the user
                                    // response
    if (userResponse[len - 1] == '\n')
        // If the last item in the response is a new line
        userResponse[len - 1] = '\0'; // Replace the item with the
                                      // '\0' character

    char *argument1 = strtok(userResponse, " "); // The user's
                                                 // input
    if (strcmp(argument1, "RQ") == 0)
    // If the user called the function 'RQ'
    {
        int process = atoi(strtok(NULL, " ")); // Retrieve the
                                               // process number
        int size = atoi(strtok(NULL, " "));    // Retrieve the
                                               // process size
        char *fit = strtok(NULL, " ");         // Retrieve the
                                               // process fit
        request(projectMemory, process, size, fit);
    }

    else if (strcmp(argument1, "RL") == 0)
    // If the user called the function 'RL'
    {
        int process = atoi(strtok(NULL, " ")); // Retrieve the
                                               // process number
        release(projectMemory, process);
    }

    else if (strcmp(argument1, "C") == 0)
        // If the user called the function 'C'
        compress(projectMemory);

    else if (strcmp(argument1, "STAT") == 0)
        // If the user called the function 'STAT'
        statistics(projectMemory);

    else if (strcmp(argument1, "X") == 0)
    // If the user called the function 'X'
    {
        printf("Program terminating...\n");
        exit(0); // Exit the program
    }
    else
        printf("Please input a valid command.\n");
}

// main
// Inputs:
//     argc - The number of arguments the user passed to the
//            program
//     argv - An array of the string the user passed into the
//            program
// Function:
//     Initially, it creates an array of "memory" requested by
//     the user, then it fills the array with "empty" slots.
//     After this, it runs an infinite loop that retrieves the
//     user's input and relays it to a function that interprets
//     the input
int main(int argc, char *argv[])
{
    memalloc = atoi(argv[1]);    // Retrieve the memory that the
                                 // user requested
    int projectMemory[memalloc]; // Create an array that has the
                                 // number of slots equal to the
                                 // ammout of memory the user
                                 // requested

    for (int i = 0; i < memalloc; i++)
        // For each slot in the array
        projectMemory[i] = -1; // Initialize the value with '-1'

    while (1)
    // INFINITE LOOP
    {
        printf("allocator> ");
        char userResponse[100]; // Creates a string that will
                                // hold the user's input
        parse_args(
            fgets(userResponse, 100, stdin), projectMemory);
    }
}