// Nicholas Rahbany
// CS 451
// prime.c

#include <ctype.h>
#include <dirent.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include <sys/time.h>
#include <time.h>
#include <unistd.h>

int checkPrimeAndPrint(unsigned long int toCheck);
int proccesNumber, priority, start = 0;
long unsigned int bigPrime = -1;

void handler(int signum)
{
    switch (signum)
    {
    case SIGTSTP:
        printf("Process %d: My priority is %d, my PID is %d: I am about to be\n", proccesNumber, priority, getpid());
        printf("suspended... Highest prime number I found is %lu.\n\n", bigPrime);
        kill(getpid(), SIGSTOP);
        break;
    case SIGCONT:
        printf("Process %d: My priority is %d, my PID is %d: I just got resumed.\n", proccesNumber, priority, getpid());
        printf("Highest prime number I found is %lu.\n\n", bigPrime);
        break;
    case SIGTERM:
        printf("Process %d: My priority is %d, my PID is %d: I completed my task\n", proccesNumber, priority, getpid());
        printf("and I am exiting. Highest prime number I found is %lu.\n\n", bigPrime);
        kill(getpid(), SIGKILL);
        break;
    }
}
void checkArgs(int argc, char *argv[])
{
    int c;
    while ((c = getopt(argc, argv, "n:p:s:")) != -1)
    {
        switch (c)
        {
        case 'n':
            proccesNumber = atoi(optarg);
            break;
        case 'p':
            priority = atoi(optarg);
            break;
        case 's':
            start = atoi(optarg);
            break;
        }
    }
}

int main(int argc, char **argv)
{
    checkArgs(argc, argv);

    struct sigaction sa;
    memset(&sa, 0, sizeof(sa));
    sa.sa_handler = handler;
    sigaction(SIGTSTP, &sa, NULL);
    sigaction(SIGCONT, &sa, NULL);
    sigaction(SIGTERM, &sa, NULL);

    printf("Process %d: My priority is %d, my PID is %d: I just got started.\n", proccesNumber, priority, getpid());
    printf("I am starting with the number %d to find the next prime\n", start);
    printf("number.\n\n");

    int numPrinted = 0;
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