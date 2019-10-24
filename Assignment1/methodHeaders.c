// Nicholas Rahbany
// CS 451
// methodHeaders.c

#include "methodHeaders.h"

// Takes in the user's arguments (from the command line) and
// sets which flags are necessary
char *checkArgs(int argc, char *argv[])
{
    int pflag, sflag, tflag, vflag, cflag = 0; // Createsthe flags that will be returned
    char *result = malloc(5 * sizeof(int));    // Defines the object this function will return
    int c;                                     // Defines the iterator of the while loop

    // While an argument is eithe ra p, s, t, v, or c
    while ((c = getopt(argc, argv, "pstvch")) != -1)
    {
        // For each value 'c' gets
        switch (c)
        {
            // if c == 'p'
        case 'p':
            // set pflag to true
            pflag = 1;
            break;
            // if c == 's'
        case 's':
            // set sflag to true
            sflag = 1;
            break;
            // if c == 't'
        case 't':
            // set tflag to true
            tflag = 1;
            break;
            // if c == 'v'
        case 'v':
            // set vflag to true
            vflag = 1;
            break;
            // if c == 'c'
        case 'c':
            // set cflag to true
            cflag = 1;
            break;
            // if c == 'h'
        case 'h':
            // Print the 'help' of this program
            printf("%s", "Type '-p' to define a specific PID\n");
            printf("%s", "Type '-s' to retrieve the current status of the specified process\n");
            printf("%s", "Type '-t' to retrieve the time this program has been running on the system\n");
            printf("%s", "Type '-v' to retrieve the size of the specified process\n");
            printf("%s", "Type '-c' to retrieve the virtual memory of the specified process\n");
            exit(1);
        default:
            // if an input was none of the options, exit the program
            abort();
        }
    }
    // string print all the flags into result
    sprintf(result, "%d %d %d %d %d", pflag, sflag, tflag, vflag, cflag);
    // return result
    return result;
}

// Determines if the PID the user inputted (if they did input one)
// is a digit or not
int checkDigit(char *digit)
{
    int size = strlen(digit);      // gets the size of the string passed in
    int result = 1;                // Defines the default output to 'true'
    for (int i = 0; i < size; i++) // for each character int the string 'digit'
    {
        // if the character is not a digit and all other characters
        // before were digits
        if (!isdigit(digit[i]) && result == 1)
        {
            result = 0; // set the return value to 'false'
        }
    }
    return result; // return 'result'
}

// This formats the seconds into HH:MM:SS
char *formatTime(int seconds)
{
    int hh = (int)(seconds / 60 * 60);                // converts seconds into hours
    int mm = (int)((seconds / 60) % 60);              // converts seconds into minutes
    int ss = (int)((seconds) % 60);                   // gets remainder of seconds from 60
    char *finalTime = malloc(50);                     // allocates memory to the string that will be returned
    sprintf(finalTime, "%02d:%02d:%02d", hh, mm, ss); // prints the format onto 'finalTime'
    return finalTime;                                 // return the formatted time
}

// This opens the file that the program will be using
char *openFile(char *filePath, char *pid)
{
    FILE *file;                  // Defines the FILE object
    file = fopen(filePath, "r"); // Opens the file with the specified path that was passed into the function
    if (file == NULL)            // If the file can't be opened
    {
        printf("Unable to open file for PID: %s\n", pid); // Print error message
        exit(1);                                          // Exit the program
    }
    char *fileContents = malloc(10000);   // Allocate memory for the file to be exported
    fscanf(file, "%[^\n]", fileContents); // Scan the contents of the file to 'fileContents'
    fclose(file);                         // Close the file
    return fileContents;                  // Return the file's contents
}

// This parses the status file and returns the required items
char *parseStatusFile(char *file)
{
    // Defines pointers for status, uptime, system time, and command line
    char *status;
    char *uptime;
    char *stime;
    char *cline;

    strtok(file, " ");           // Start iterating through the file
    for (int i = 0; i < 12; i++) // For each integer between 0 and 12
    {
        char *value = strtok(NULL, " "); // Define the temporary variable as 'value'
        if (i == 0)                      // if the iterator is 0
        {
            cline = value; // Define command line as the current value
        }
        if (i == 1) // if the iterator is 1
        {
            status = value; // define status as the current value
        }
        else if (i == 11) // if the iterator is 11
        {
            uptime = value; // define uptime as the current value
        }
        else if (i == 12) // if the iterator is 12
        {
            stime = value; // define stime as the current value
        }
    }
    char *result = malloc(50);                                    // allocate memory for the result
    sprintf(result, "%s %s %s %s", status, uptime, stime, cline); // print the string to 'result'
    return result;                                                // return result
}

// This parses the first item from a file
char *parseFirstItemFromFile(char *file)
{
    return strtok(file, " "); // return the first item from the file
}

// This verify's the PID the user input (if the
// user input a PID)
int verifyPath(char *filePath)
{
    FILE *file;                  // creates file object we will be using
    file = fopen(filePath, "r"); // sets the 'file' variable to the file at the specified path
    if (file == NULL)            // if the path doesn't exist
    {
        return 0; // return 'false'
    }
    return 1; // return 'true'
}