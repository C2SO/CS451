// Nicholas Rahbany
// CS 451
// 5ps.c

// methodHeaders.h contains all the functions and other inputs required for this file
#include "methodHeaders.h"

// This is the main program
int main(int argc, char *argv[])
{
	const int TICKS = sysconf(_SC_CLK_TCK); // Used to determine the amount of ticks the system runs on

	char pid[5] = "1";							  // Defines the default PID to '1'
	char printString[100];						  // creates the final string we will be returning
	char *filePath = malloc(30);				  // Creates the filepath the project will be using
	int pidInString, touched = 0;				  // defines if the pid is already coppied into 'printstring' and if a flag has been touched
	char *formattedArgs = checkArgs(argc, argv);  // Holds the 'boolean' values for the flags the user passes in
	int pflag = atoi(strtok(formattedArgs, " ")); // defines the flag for p
	int sflag = atoi(strtok(NULL, " "));		  // defines the flag for s
	int tflag = atoi(strtok(NULL, " "));		  // defines the flag for t
	int vflag = atoi(strtok(NULL, " "));		  // defines the flag for v
	int cflag = atoi(strtok(NULL, " "));		  // defines the flag for c

	free(formattedArgs); // free's the momory allocated to 'formattedArgs'

	if (pflag == 1) // if the user called -p in the command line
	{
		touched = 1;				   // a flag has been touched
		int pidInput = 0;			   // Defines the 'boolean' to see if the PID is valid
		for (int i = 1; i < argc; i++) // for every parameter the user gave
		{
			if (strstr(argv[i], "-") == NULL) // if that parameter does not have a '-'
			{
				sprintf(filePath, "%s%s%s", "/proc/", argv[i], "/stat"); // define a file path with that parameter
				// if the supposed PID is valid, is a number, and exists
				if (argv[i] != NULL && checkDigit(argv[i]) == 1 && verifyPath(filePath) == 1)
				{
					strcpy(pid, argv[i]);		  // define the PID from the parameters into the 'pid' variable
					strcat(printString, argv[i]); // add the PID to the final output
					strcat(printString, ": ");	// Add ': ' to the final output
					pidInput = 1;				  // The PID the user input is valid
					pidInString = 1;			  // The PID is in the final output
				}
			}
		}
		if (pidInput == 0) // If the PID the user defined is not valid
		{
			printf("%s\n", "You have not provided a valid PID"); // Print an error message
			exit(1);											 // Exit the program
		}
		if (!(sflag == 1 || tflag == 1 || vflag == 1 || cflag == 1)) // If no other flags are called
		{
			exit(1); // Exit the program
		}
	}

	// For flags s, t, v, and c
	if (sflag == 1 || tflag == 1 || vflag == 1 || cflag == 1)
	{
		sprintf(filePath, "%s%s%s", "/proc/", pid, "/stat"); // Define the file path
		touched = 1;										 // a flag has been touched
		char *file = openFile(filePath, pid);				 // Open the file from the created file path
		char *fileParse = parseStatusFile(file);			 // Parse the status file
		char *status = strtok(fileParse, " ");				 // Define status as the first item in the 'fileParse' string
		int uptime = atoi(strtok(NULL, " "));				 // Define uptime as the second item in the 'fileParse' string
		int systemTime = atoi(strtok(NULL, " "));			 // Define systemTime as the third item in the 'fileParse' string
		char *virtualMemory = strtok(NULL, " ");			 // Define cirtualMemory as the fourth item in the 'fileParse' string
		if (pidInString == 0)								 // If the current PID is not in the final output
		{
			strcat(printString, pid);  // Add the current PID to the final output
			strcat(printString, ": "); // Add ': ' to the final output
			pidInString = 1;		   // The PID is in the final output
		}
		if (sflag == 1) // If the user called -s in the command line
		{
			strcat(printString, status); // Add the status of the PID to the final output
			strcat(printString, " ");	// Add a space to the final output
		}
		if (tflag == 1) // If the user calles -t in the command line
		{
			// Add the ammout of ticks in uptime and system, then divide it by the number of ticks per second in the system
			int calculatedTime = ((uptime + systemTime) / TICKS);
			char *formattedTime = formatTime(calculatedTime); // format 'calculatedTime'
			strcat(printString, "time=");					  // Add 'time=' to the final output
			strcat(printString, formattedTime);				  // Add the formatted time to the output
			strcat(printString, " ");						  // add a space to the final output
			free(formattedTime);							  // Free the memory allocated for the time
		}
		if (vflag == 1) // If the user calls -v in the command line
		{
			sprintf(filePath, "%s%s%s", "/proc/", pid, "/statm"); // Overwrite the old filepath with the new one
			char *statm = openFile(filePath, pid);				  // Open the new file
			char *statmParsed = parseFirstItemFromFile(statm);	// Parse the statm file
			strcat(printString, "sz=");							  // Add 'sz=' to the final output
			strcat(printString, statmParsed);					  // Add the size to the final output
			strcat(printString, " ");							  // Add a space to the final output
			free(statm);										  // Fre the memory allocated for 'statm'
		}
		if (cflag == 1) // If the user calls -c in the command line
		{
			strcat(printString, "[");			// Add '[' to the final output
			strcat(printString, virtualMemory); // Add the virtual memory to the final output
			strcat(printString, "]");			// Add ']' to the final output
		}

		free(file);		 // Free the memory allocated to 'file'
		free(fileParse); // Free the memory allocated to 'fileParse'
	}

	printf("%s", printString); // Print the final string
	if (touched == 1)		   // If there was an output
	{
		printf("%s", "\n"); // Print a new line
	}

	return 0; // Exit the program
}