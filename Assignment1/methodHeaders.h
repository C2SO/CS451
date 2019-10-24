// Nicholas Rahbany
// CS 451
// methodHeaders.h

#include <ctype.h>
#include <dirent.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>

// Takes in the user's arguments (from the command line) and 
// sets which flags are necessary
char *checkArgs(int argc, char *argv[]);

// Determines if the PID the user inputted (if they did input one)
// is a digit or not
int checkDigit(char *digit);

// This formats the seconds into HH:MM:SS
char *formatTime(int seconds);

// This opens the file that the program will be using
char *openFile(char *filePath, char *pid);

// This parses the status file and returns the required items
char *parseStatusFile(char *file);

// This parses the first item from a file
char *parseFirstItemFromFile(char *file);

// This verify's the PID the user input (if the
// user input a PID)
int verifyPath(char *filePath);