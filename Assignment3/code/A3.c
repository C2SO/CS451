// Nicholas Rahbany
// A3.c
// CS451

#include <stdio.h>
#include <pthread.h>
#include <unistd.h>
#include <semaphore.h>
#include <stdlib.h>
#include "sem.h"
#include "elevator.h"
#include "person.h"

// Initializing global variables
// numberOfPeople will be the number of people the program will handle
// maxWanderTime will be the max wander time the program will handle
// numberOfFloors will be the number of floors the program will handle
int numberOfPeople, maxWanderTime, numberOfFloors = -1;

// parse_args parses the arguments the user passes in
void parse_args(int argc, char *argv[], int *people,
                int *wanderTime, int *floors);

// Creates the person and elevator structs
int main(int argc, char *argv[])
{
    // Create all the semaphores
    sem_create_all();

    // Parse the arguments the user passed in
    parse_args(argc, argv, &numberOfPeople, &maxWanderTime,
               &numberOfFloors);

    // Determine if a file was passed in
    if (numberOfPeople == -1 || numberOfFloors == -1 || maxWanderTime == -1)
    {
        printf("Please define all arguments. Terminating process...\n");
        exit(0);
    }

    // Create elevator struct and define its base values
    struct elevator *elev = (struct elevator *)malloc(sizeof(struct elevator));
    elev->maxWaitTime = maxWanderTime;
    elev->totalFloors = numberOfFloors;
    elev->direction = 1; // 1 is up, 0 is down
    elev->currFloor = 0;
    elev->nextFloor = 1;

    // Create an array of people and an array of people threads
    struct person *people[numberOfPeople - 1];
    pthread_t peopleThreads[numberOfPeople - 1];

    // Create person structs and pass it into the array
    for (int i = 0; i < numberOfPeople; i++)
    {
        people[i] = (struct person *)malloc(sizeof(struct person));
        people[i] = person_init(i);
    }

    // Create elevator and elevator thread
    pthread_t thElevator;
    pthread_create(&thElevator, NULL, elevator_travel, (void *)elev);

    // Create people threads
    for (int i = 0; i < numberOfPeople; i++)
    {
        pthread_create(&peopleThreads[i], NULL, enter_building,
                       (void *)people[i]);
    }

    // Wait for all threads to terminate before ending program
    pthread_join(thElevator, NULL);
    for (int i = 0; i < numberOfPeople - 1; i++)
    {
        pthread_join(peopleThreads[i], NULL);
    }
}

// parse_args parses the arguments the user passes in
void parse_args(int argc, char *argv[], int *people,
                int *wanderTime, int *floors)
{
    int opt;
    // Read the arguments the user passed in
    while ((opt = getopt(argc, argv, "p:w:f:")) != -1)
    {
        switch (opt)
        {
        case 'p':
            *people = atoi(optarg);
            break;
        case 'w':
            *wanderTime = atoi(optarg);
            break;
        case 'f':
            *floors = atoi(optarg);
            break;
        default:
            // Check to see if the current option is any of the
            // default options
            if (optopt == 'p')
            {
                exit(0);
            }
            if (optopt == 'w')
            {
                exit(0);
            }
            if (optopt == 'f')
            {
                exit(0);
            }
            break;
        }
    }
}