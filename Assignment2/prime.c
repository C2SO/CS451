// Nicholas Rahbany
// CS 451
// prime.c

#include <stdio.h>
#include <stdlib.h>

long unsigned int calculatedPrime = 123400003;
void handler(int signum);
int checkPrimeAndPrint(unsigned long int toCheck);

int main(int argc, char **argv)
{

    struct sigaction sa_suspend;
    memset(&sa, 0, sizeof(sa));
    sa.sa_handler = suspend_handler;
    sigaction(SIGTSTP, &sa, NULL);

    struct sigaction sa_continue;
    memset(&sa, 0, sizeof(sa));
    sa.sa_handler = continue_handler;
    sigaction(SIGCONT, &sa, NULL);

    struct sigaction sa_terminate;
    memset(&sa, 0, sizeof(sa));
    sa.sa_handler = terminate_handler;
    sigaction(SIGQUIT, &sa, NULL);

    int numPrinted = 0;
    long unsigned int numberToCheck = calculatedPrime + 1;
    while (numPrinted < 10)
    {
        if (checkPrimeAndPrint(numberToCheck) == 1)
        {
            printf("prime number is %lu \n", numberToCheck);
            numPrinted++;
        }
        numberToCheck++;
    }
}

void suspend_handler(int signum)
{
    // Write handler code here.
}

void continue_handler(int signum)
{
    // Write handler code here.
}

void terminate_handler(int signum)
{
    // Write handler code here.
}

int checkPrimeAndPrint(unsigned long int toCheck)
{
    unsigned long int i = 2;
    int prime = 1;
    while (prime == 1 && i < toCheck / 2)
    {
        if (toCheck % i == 0)
            prime = 0;
        i++;
    }
    return (prime);
}