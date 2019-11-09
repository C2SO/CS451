// Nicholas Rahbany
// CS 451
// A2.c

#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/time.h>
#include <unistd.h>

int **fileData;                                                               // This will be a resizable matrix that we are going to be mallocing later on
int currProcess = -1;                                                         // Initialize the current process as -1
int timePassed, fileLines, terminatedProcesses, terminate, printTimeBool = 0; // Initialize the time passed,
                                                                              // the lines in the input file, the number of terminated processes,
                                                                              // the terminate boolean, and the print time boolean to -1
int *waitList; // This is a waitlist array that will be populated later

// This function opens the file, and returns the file object
FILE *open_file(char *fileName)
{
    FILE *primeFile = fopen(fileName, "r");
    if (primeFile == NULL)
    {
        printf("Cannot open file \n");
        exit(0);
    }
    return primeFile;
}

// This function checks if the time was printed yet for this second
// If so, it will not do anything. If not, it will do nothing.
void print_time()
{
    if (printTimeBool == 0)
    {
        printf("Scheduler: Time Now: %d Seconds\n", timePassed);
    }
    printTimeBool = 1;
}

// This function counts lines in a file.
// It is passed in a file name, and will open that file and use it.
int count_lines(char *fileName)
{
    int lines = 1;
    FILE *tempFile = open_file(fileName);
    char chr = getc(tempFile);

    //For each line in the file, increase the iterator
    while (chr != EOF)
    {
        if (chr == '\n')
        {
            lines++;
        }
        chr = getc(tempFile);
    }
    fclose(tempFile);

    //Returns the number of lines in the file
    return lines;
}

//This function opens the file, and then it converts the file into a matrix
int **file_to_array(char *fileName)
{
    int **fileArray = malloc(fileLines * sizeof(double)); // Allocate memory for the matrix

    //For each line in the file, allocate the memory for the rows
    for (int i = 0; i < fileLines; ++i)
    {
        fileArray[i] = malloc(5 * sizeof(double));
    }

    //Place all the items from the file into fileArray
    FILE *primeFile = open_file(fileName);
    int iterator = 0;
    for (int i = 0; i < fileLines; i++)
    {
        for (int j = 0; j < 4; j++)
        {
            if (!fscanf(primeFile, "%d", &fileArray[i][j]))
            {
                break;
            }
        }
    }
    return fileArray;
    free(fileArray);
}

// This was used for debugging purposes, this prints the wait queue
void print_wait_queue()
{
    for (int i = 0; i < fileLines; i++)
    {
        printf("Wait list[%d]: %d\n", i, waitList[i]);
    }
}

// This returns the process line that the passed in process is on
int get_process_line(int process)
{
    for (int i = 0; i < fileLines; i++)
    {
        if (fileData[i][0] == process)
        {
            return i;
        }
    }
    return -1;
}

// This returns the priority of the current process
int get_priority()
{
    return fileData[get_process_line(currProcess)][3];
}

// This returns the 10-digit number prime.c will be using
double get_number()
{
    return 1000000000;
}

// This function's purpose is to suspend a current process, to resume a process, and/or to start a process
void start_process(int processID)
{
    // If the current process is valid, and there is a valid PID for the process
    if (currProcess != -1 && fileData[get_process_line(currProcess)][4] != -1)
    {
        int processPID = fileData[get_process_line(currProcess)][4];
        kill(processPID, SIGTSTP); // Suspend the current process
        if (printTimeBool == 0)
        {
            print_time();
            printf("Suspending Process %d (Pid %d) ", currProcess, fileData[get_process_line(currProcess)][4]);
        }
        else
        {
            printf("and Suspending Process %d (Pid %d) \n", currProcess, fileData[get_process_line(currProcess)][4]);
        }
    }
    // If a process has been terminated recently, the new process is valid, AND there is a valid PID for the process
    if (terminate == 1 && currProcess != -1 && fileData[get_process_line(currProcess)][4] != -1)
    {
        kill(fileData[get_process_line(currProcess)][4], SIGCONT); // Resume the current process
        if (printTimeBool == 0)
        {
            print_time();
            printf("Resuming Process %d (Pid %d) ", currProcess, fileData[get_process_line(currProcess)][4]);
        }
        else
        {
            printf("and Resuming Process %d (Pid %d) \n", currProcess, fileData[get_process_line(currProcess)][4]);
        }
        terminate = 0;
    }
    else // The process has not been started yet
    {
        terminate = 0;
        currProcess = processID; // Set the current running process to the new process
        int child_pid;
        child_pid = fork();
        if (child_pid == 0) // Child process
        {
            char str[11];
            sprintf(str, "%fl", get_number());
            char str2[5];
            sprintf(str2, "%d", processID);
            char str3[5];
            sprintf(str3, "%d", get_priority());
            execlp("./prime.out", "./prime.out", "-s", str, "-p", str3, "-n", str2, NULL); // Start prime.c for this process
        }
        else // Parent process
        {
            fileData[get_process_line(processID)][4] = child_pid; // Store the child's PID for future use
            if (printTimeBool == 0)
            {
                print_time();
                printf("Scheduling to Process %d (Pid %d) \n", currProcess, fileData[get_process_line(currProcess)][4]);
            }
            else
            {
                printf("and Scheduling to Process %d (Pid %d) \n", currProcess, fileData[get_process_line(currProcess)][4]);
            }
        }
    }
}

// This file compares two processes and returns the one with the highest priority
int compare_priority(int pid1, int pid2)
{
    // If the priorities differ, return the one with the higher priority
    if (fileData[pid1][3] < fileData[pid2][3])
    {
        return fileData[pid1][0];
    }
    else if (fileData[pid1][3] > fileData[pid2][3])
    {
        return fileData[pid2][0];
    }
    else // If the priorities are the same, return the one that was receied first
    {
        if (fileData[pid1][1] > fileData[pid2][1])
        {
            return pid2;
        }
        else
        {
            return pid1;
        }
    }
}

// This function updates the waitlist array, by checking which one has a higher priority, then placing it into the array
void update_wait_list_array(int pid1, int pid2, int arrayIndex)
{
    if (pid1 == -1) // If there is process for pid1, add pid2 to the passed in array index
    {
        waitList[arrayIndex] = pid2;
    }
    else if (compare_priority(pid1, pid2) == pid2) // If the priority is pid2
    {
        // Place pid2 where pid1 was, and search for where pid1 should go in the array
        waitList[arrayIndex] = pid2;
        update_wait_list_array(waitList[arrayIndex + 1], pid1, arrayIndex + 1);
    }
    else // pid1 has the priority
    {
        // Compare pid2 to the next item in the waitlist
        update_wait_list_array(waitList[arrayIndex + 1], pid2, arrayIndex + 1);
    }
}

// This function is the preliminary call for update_wait_list_array()
void add_to_queue(int process)
{
    update_wait_list_array(waitList[0], process, 0);
}

// This checks if the passed in process is in the waitList array
int process_not_in_wait(int process)
{
    for (int i = 0; i < fileLines; i++)
    {
        if (waitList[i] == process)
        {
            return 0;
        }
    }
    return 1;
}

// This shifts the waitlist all up by one, placing an item to a slot infront of it
void shift_wait_list()
{
    for (int i = 0; i < fileLines - 1; i++)
    {
        waitList[i] = waitList[i + 1];
    }
    waitList[fileLines] = -1;
}

// This function returns the current priority process, wether it be in the waitlist or in the text file
int get_priority_process()
{
    int priorityProcess = currProcess;
    if (priorityProcess == -1) // If the current process is invalid, set it as the top item in the waitlist
    {
        priorityProcess = waitList[0];
    }
    // For every line in in the file
    // If there is no valid priority, define it as the first process to be entered
    // If the processes are to be "added" at the current time
    // Compare the priorities. If the incoming ID is the priority and it is not in the waiting queue,
    //      add the currennt priority to the queue and set the other as the proiority process
    // Otherwise, add the process to the queue
    for (int i = 0; i < fileLines; i++)
    {
        int incomingID = fileData[i][0];
        if (priorityProcess != -1)
        {
            int priorityLine = get_process_line(priorityProcess);
            int priorityID = fileData[priorityLine][0];
            if (fileData[i][1] == timePassed)
            {
                if (compare_priority(priorityID, incomingID) == incomingID && process_not_in_wait(incomingID) == 1)
                {
                    add_to_queue(priorityID);
                    priorityProcess = incomingID;
                }
                else
                {
                    add_to_queue(incomingID);
                }
            }
        }
        else
        {
            priorityProcess = incomingID;
        }
    }
    // Return the process with the highest priority
    return priorityProcess;
}

// This function terminates a process when it has run the specified bursts
void terminate_process()
{
    kill(fileData[get_process_line(currProcess)][4], SIGTERM); // Sends the terminate signal
    if (printTimeBool == 0)
    {
        print_time();
        printf("Terminating Process %d (Pid %d) ", currProcess, fileData[get_process_line(currProcess)][4]);
    }
    else
    {
        printf("and Terminating Process %d (Pid %d) \n", currProcess, fileData[get_process_line(currProcess)][4]);
    }
    currProcess = -1; // Defines the current process as "nothing"
    terminatedProcesses++;
    if (terminatedProcesses == fileLines) // If there are as many terminated processes as there are items that were entered, then exit the program
    {
        printf("\n\nAll processes have successfully run!\n");
        exit(1);
    }
}

// This makes sure that every time a second has passed, the current function is working properly
void tick_curr_process()
{
    for (int i = 0; i < fileLines; i++)
    {
        // If the item we are looking at is the current process and no process has been terminated yet
        if (fileData[i][0] == currProcess && terminate == 0)
        {
            // Set the time for the process to itself minus one
            int time = fileData[i][2] - 1;
            fileData[i][2] = time;
            if (time == 0) // If there is no time left for the process, termiante it
            {
                terminate_process();
                terminate = 1;
            }
        }
        // printf("Time[%d]: %d\n", i, fileData[i][2]);
    }
}

// This function checks the current process to see if there are any processes that take presidence over it
// If so, it calls 'start_process' to start the priority process
void check_current_process()
{
    int priorityProcess = get_priority_process();
    if (priorityProcess != currProcess) // If the current priority process is not the one that is running
    {
        if (priorityProcess == waitList[0])
        {
            shift_wait_list();
        }
        start_process(priorityProcess);
    }
}

// This function is run every second
// First it adds a second to the time passed for this program
// Next, it 'ticks' the current process, if there is one
// Finally, it checks if the current process needs to be switched
// The printing at the end is to normalize the printing from the scheduler
void timer_handler(int signal)
{
    timePassed++;
    // printf("%d Seconds\n\n", timePassed);
    if (currProcess != -1)
    {
        tick_curr_process();
    }
    check_current_process();
    if (printTimeBool == 1)
    {
        printf("\n");
        printTimeBool = 0;
    }
}

// This function retrieves the file's data and number of lines
// then it allocates memory for the waitlist.
// It is also home to the timer that is used to count the seconds!
void read_file(char *fileName)
{
    fileLines = count_lines(fileName);
    fileData = file_to_array(fileName);

    waitList = malloc(fileLines * sizeof(int));
    for (int i = 0; i < fileLines; i++)
    {
        waitList[i] = -1;
    }

    struct itimerval timer;
    timer.it_value.tv_sec = 1;
    timer.it_value.tv_usec = 0;
    timer.it_interval.tv_sec = 1;
    timer.it_interval.tv_usec = 0;
    setitimer(ITIMER_REAL, &timer, NULL);

    struct sigaction sa;
    memset(&sa, 0, sizeof(sa));
    sa.sa_handler = timer_handler;
    sigaction(SIGALRM, &sa, NULL);

    while (1)
    {
    }

    free(waitList);
}

// This is the first function that is ran when A2.c is called
// It adds a new line, retrieves the file name, then passes everything off to 'read_file'
int main(int argc, char *argv[])
{
    printf("\n");
    char *fileName = argv[1];
    read_file(fileName);
}