// Nicholas Rahbany
// CS 451
// prime.c

#include <math.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

int proccesNumber, priority, start = -1; // Initialize the process number, process priority, and the start number to -1
long unsigned int bigPrime = -1;         // Set the prime number we will be printing to -1

// This function checks what signal the process receives, then decides what to do from there.
void handler(int signum)
{
    switch (signum) // For each signal sent
    {
    case SIGTSTP: // If the process is to suspend...
        printf("Process %d: My priority is %d, my PID is %d: I am about to be\n", proccesNumber, priority, getpid());
        printf("suspended... Highest prime number I found is %lu.\n\n", bigPrime);
        kill(getpid(), SIGSTOP);
        break;
    case SIGCONT: // If the process is to resume...
        printf("Process %d: My priority is %d, my PID is %d: I just got resumed.\n", proccesNumber, priority, getpid());
        printf("Highest prime number I found is %lu.\n\n", bigPrime);
        break;
    case SIGTERM: // If the process is to terminate...
        printf("Process %d: My priority is %d, my PID is %d: I completed my task\n", proccesNumber, priority, getpid());
        printf("and I am exiting. Highest prime number I found is %lu.\n\n", bigPrime);
        kill(getpid(), SIGKILL);
        break;
    }
}

//This function checks the arguments passed into the process when it is called
void checkArgs(int argc, char *argv[])
{
    int c;
    while ((c = getopt(argc, argv, "n:p:s:")) != -1)
    {
        switch (c) // for each argument passed in
        {
        case 'n': // Set the process number
            proccesNumber = atoi(optarg);
            break;
        case 'p': // Set the process priority
            priority = atoi(optarg);
            break;
        case 's': // Set the process' start number
            start = atoi(optarg);
            break;
        }
    }
}

// The main function checks the args, and sets an infinite loop to check if a number is prime
int main(int argc, char **argv)
{
    checkArgs(argc, argv); // Check arguments

    printf("Process %d: My priority is %d, my PID is %d: I just got started.\n", proccesNumber, priority, getpid());
    printf("I am starting with the number %d to find the next prime\n", start);
    printf("number.\n\n");

    // Set the sigaction object to wait for actions from A2.c
    struct sigaction sa;
    memset(&sa, 0, sizeof(sa));
    sa.sa_handler = handler;
    sigaction(SIGTSTP, &sa, NULL);
    sigaction(SIGCONT, &sa, NULL);
    sigaction(SIGTERM, &sa, NULL);

    // Starting with the input number + 1
    // While a number is less than the sqare root of the input number + 1
    // If there is a remainder if divided, then it is not a ptime
    // Else, it is a prime
    long unsigned int possibleBigPrime = start + 1;
    while (1)
    {
        int notPrime = 0;
        for (int counter = 3; counter <= sqrt(possibleBigPrime); counter += 2)
        {
            if ((possibleBigPrime % counter) == 0)
            {
                notPrime = 1;
            }
        }
        if (notPrime == 0)
        {
            bigPrime = possibleBigPrime;
        }
        possibleBigPrime++;
    }
}