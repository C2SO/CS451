// Nicholas Rahbany
// CS 451
// A2.c

#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <ctype.h>
#include <string.h>
#include <time.h>
#include <unistd.h>
#include <signal.h>
#include <sys/time.h>

int **fileData;
int currProcess = -1;
int timePassed, fileLines = 0;
int *waitList;

FILE *openFile(char *fileName)
{
    FILE *primeFile = fopen(fileName, "r");
    if (primeFile == NULL)
    {
        printf("Cannot open file \n");
        exit(0);
    }
    return primeFile;
}

int countLines(char *fileName)
{
    int lines = 1;
    FILE *tempFile = openFile(fileName);
    char chr = getc(tempFile);
    while (chr != EOF)
    {
        if (chr == '\n')
        {
            lines++;
        }
        chr = getc(tempFile);
    }
    fclose(tempFile);
    return lines;
}

int **fileToArray(char *fileName)
{
    int **fileArray = malloc(fileLines * sizeof(double));
    for (int i = 0; i < fileLines; ++i)
    {
        fileArray[i] = malloc(4 * sizeof(double));
    }
    FILE *primeFile = openFile(fileName);
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
}

void startProcess(int processID)
{
    if (currProcess != processID && currProcess != -1)
    {
        printf("Stopping process: %d\n", currProcess);
    }
    currProcess = processID;
    printf("Started process %d\n", currProcess);
}

int comparePriority(int pid1, int pid2)
{
    if (fileData[pid1][3] < fileData[pid2][3])
    {
        return fileData[pid1][0];
    }
    return fileData[pid2][0];
}

void updateWaitListArray(int pid1, int pid2, int arrayIndex)
{
    if (pid1 == -1)
    {
        waitList[arrayIndex] = pid2;
        printf("Added %d to slot %d\n", pid2, arrayIndex);
    }
    else if (comparePriority(pid1, pid2) == pid2)
    {
        waitList[arrayIndex] = pid2;
        printf("Added %d to slot %d\n", pid2, arrayIndex);
        updateWaitListArray(waitList[arrayIndex + 1], pid1, arrayIndex + 1);
    }
    else
    {
        updateWaitListArray(waitList[arrayIndex + 1], pid2, arrayIndex + 1);
    }
}

void printWaitQueue()
{
    for (int i = 0; i < fileLines; i++)
    {
        printf("Wait list[%d]: %d\n", i, waitList[i]);
    }
}

void addToQueue(int process)
{
    updateWaitListArray(waitList[0], process, 0);
    printWaitQueue();
}

int getPriorityProcess()
{
    int priorityProcess = currProcess;
    for (int i = 0; i < fileLines; i++)
    {
        if (priorityProcess != -1)
        {
            if (fileData[i][1] == timePassed)
            {
                if (fileData[priorityProcess][3] > fileData[i][3])
                {
                    addToQueue(fileData[priorityProcess][0]);
                    priorityProcess = fileData[i][0];
                }
            }
        }
        else
        {
            priorityProcess = fileData[i][0];
        }
    }
    return priorityProcess;
}

void checkCurrentProcess()
{
    ++timePassed;
    int priorityProcess = getPriorityProcess();
    startProcess(priorityProcess);
}

void timer_handler(int signal)
{
    checkCurrentProcess();
}

void readFile(char *fileName)
{
    fileLines = countLines(fileName);
    fileData = fileToArray(fileName);

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
}

char *getFileName(char *argv[])
{
    return argv[1];
}

int main(int argc, char *argv[])
{
    char *fileName = getFileName(argv);
    readFile(fileName);
}