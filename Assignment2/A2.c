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
int timePassed, fileLines, terminatedProcesses, terminate, printTimeBool = 0;
int *waitList;
char *timeString[100];

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

void printTime()
{
    if (printTimeBool == 0)
    {
        printf("Scheduler: Time Now: %d Seconds\n", timePassed);
    }
    printTimeBool = 1;
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
        fileArray[i] = malloc(5 * sizeof(double));
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

void printWaitQueue()
{
    for (int i = 0; i < fileLines; i++)
    {
        printf("Wait list[%d]: %d\n", i, waitList[i]);
    }
}

int getProcessLine(int process)
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

int getPriority()
{
    return fileData[getProcessLine(currProcess)][3];
}

double getNumber()
{
    return 1000000000;
}

void startProcess(int processID)
{
    // printf("currProcess: %d\n newProcess: %d\n", currProcess, processID);
    if (currProcess != -1 && fileData[getProcessLine(currProcess)][4] != -1)
    {
        int processPID = fileData[getProcessLine(currProcess)][4];
        kill(processPID, SIGTSTP);
        if (printTimeBool == 0)
        {
            printTime();
            printf("Suspending Process %d (Pid %d) ", currProcess, fileData[getProcessLine(currProcess)][4]);
        }
        else
        {
            printf("and Suspending Process %d (Pid %d) \n", currProcess, fileData[getProcessLine(currProcess)][4]);
        }
    }

    if (terminate == 1 && currProcess != -1 && fileData[getProcessLine(currProcess)][4] != -1)
    {
        kill(fileData[getProcessLine(currProcess)][4], SIGCONT);
        if (printTimeBool == 0)
        {
            printTime();
            printf("Resuming Process %d (Pid %d) ", currProcess, fileData[getProcessLine(currProcess)][4]);
        }
        else
        {
            printf("and Resuming Process %d (Pid %d) \n", currProcess, fileData[getProcessLine(currProcess)][4]);
        }
        terminate = 0;
    }
    else
    {
        terminate = 0;
        currProcess = processID;
        int child_pid;
        child_pid = fork();
        if (child_pid == 0)
        {
            // printf("Starting process %d\n", getpid());
            char str[11];
            sprintf(str, "%fl", getNumber());
            char str2[5];
            sprintf(str2, "%d", processID);
            char str3[5];
            sprintf(str3, "%d", getPriority());
            execlp("./prime.out", "./prime.out", "-s", str, "-p", str3, "-n", str2, NULL);
        }
        else
        {
            fileData[getProcessLine(processID)][4] = child_pid;
            if (printTimeBool == 0)
            {
                printTime();
                printf("Scheduling to Process %d (Pid %d) \n", currProcess, fileData[getProcessLine(currProcess)][4]);
            }
            else
            {
                printf("and Scheduling to Process %d (Pid %d) \n", currProcess, fileData[getProcessLine(currProcess)][4]);
            }
        }
    }
}

int comparePriority(int pid1, int pid2)
{
    if (fileData[pid1][3] < fileData[pid2][3])
    {
        return fileData[pid1][0];
    }
    else if (fileData[pid1][3] > fileData[pid2][3])
    {
        return fileData[pid2][0];
    }
    else
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

void updateWaitListArray(int pid1, int pid2, int arrayIndex)
{
    if (pid1 == -1)
    {
        waitList[arrayIndex] = pid2;
    }
    else if (comparePriority(pid1, pid2) == pid2)
    {
        // Pid2 has higher priority than pid1
        waitList[arrayIndex] = pid2;
        updateWaitListArray(waitList[arrayIndex + 1], pid1, arrayIndex + 1);
    }
    else
    {
        //pid1 has higher priority over pid2
        updateWaitListArray(waitList[arrayIndex + 1], pid2, arrayIndex + 1);
    }
}

void addToQueue(int process)
{
    updateWaitListArray(waitList[0], process, 0);
}

int processNotInWait(int process)
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

void shiftWaitList()
{
    for (int i = 0; i < fileLines - 1; i++)
    {
        waitList[i] = waitList[i + 1];
    }
    waitList[fileLines] = -1;
}

int getPriorityProcess()
{
    int priorityProcess = currProcess;
    if (priorityProcess == -1)
    {
        priorityProcess = waitList[0];
    }
    // printf("\n");
    // printWaitQueue();
    // printf("\nCurrent Piority: %d\n", priorityProcess);
    for (int i = 0; i < fileLines; i++)
    {
        int incomingID = fileData[i][0];
        if (priorityProcess != -1)
        {
            int priorityLine = getProcessLine(priorityProcess);
            int priorityID = fileData[priorityLine][0];
            if (fileData[i][1] == timePassed)
            {
                if (comparePriority(priorityID, incomingID) == incomingID && processNotInWait(incomingID) == 1)
                {
                    // printf("\nAdding priorityID %d to queue\n", priorityID);
                    addToQueue(priorityID);
                    priorityProcess = incomingID;
                }
                else
                {
                    // printf("\nAdding incomingID %d to queue\n", incomingID);
                    addToQueue(incomingID);
                }
            }
        }
        else
        {
            priorityProcess = incomingID;
        }
    }
    // printf("\n");
    // printWaitQueue();
    return priorityProcess;
}

void terminateProcess()
{
    kill(fileData[getProcessLine(currProcess)][4], SIGTERM);
    if (printTimeBool == 0)
    {
        printTime();
        printf("Terminating Process %d (Pid %d) ", currProcess, fileData[getProcessLine(currProcess)][4]);
    }
    else
    {
        printf("and Terminating Process %d (Pid %d) \n", currProcess, fileData[getProcessLine(currProcess)][4]);
    }
    currProcess = -1;
    terminatedProcesses++;
    if (terminatedProcesses == fileLines)
    {
        printf("\n\nAll processes have successfully run!\n");
        exit(1);
    }
}

void tickCurrProcess()
{
    for (int i = 0; i < fileLines; i++)
    {
        if (fileData[i][0] == currProcess && terminate == 0)
        {
            int time = fileData[i][2] - 1;
            fileData[i][2] = time;
            if (time == 0)
            {
                terminateProcess();
                terminate = 1;
            }
        }
        // printf("Time[%d]: %d\n", i, fileData[i][2]);
    }
}

void checkCurrentProcess()
{
    int priorityProcess = getPriorityProcess();
    if (priorityProcess != currProcess)
    {
        if (priorityProcess == waitList[0])
        {
            shiftWaitList();
        }
        startProcess(priorityProcess);
    }
    // printf("\n");
}

void timer_handler(int signal)
{
    timePassed++;
    // printf("%d Seconds\n\n", timePassed);
    if (currProcess != -1)
    {
        tickCurrProcess();
    }
    checkCurrentProcess();
    if (printTimeBool == 1)
    {
        printf("\n");
        printTimeBool = 0;
    }
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
        if (terminatedProcesses == fileLines)
        {
            exit(1);
        }
    }
}

char *getFileName(char *argv[])
{
    return argv[1];
}

int main(int argc, char *argv[])
{
    printf("\n");
    char *fileName = getFileName(argv);
    readFile(fileName);
}