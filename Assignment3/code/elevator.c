// Nicholas Rahbany
// CS451
// elevator.c

#include <stdio.h>
#include <pthread.h>
#include <unistd.h>
#include <semaphore.h>
#include <stdlib.h>
#include "sem.h"
#include "elevator.h"

// Global Variables
extern int numberOfFloors; // Number of floors
sem_t printMutex; // Used for printing
sem_t floorCall[ARRAY_SIZE]; // Array of semaphores for each floor
sem_t pwMutex; // Protects the 'peopleWaiting' array
int peopleWaiting[ARRAY_SIZE]; // Stores locations of peoples'
                               // desired floors

void *elevator_travel(void *args);
int change_direction(int direction, int currFloor);
void no_movement(int finalTrip, int sleepTime);
void start_upward();
void start_downward();
void floor_stop(int currFloor);

// Manages the logic for the elevator moving up and down
void *elevator_travel(void *args)
{
    struct elevator *elev = (struct elevator *)args;

    int movement, finalTrip = 0;
    // movement is a boolean which stores of people have gotten 
    // on/off the elevator on one floor
    // finalTrip stores a boolean which determines a trip as final
    // or not

    // Start moving the elevator upward
    start_upward();

    // Infinite loop until the process kills itself
    while (1)
    {
        sleep(1); // The "padding" for moving to each floor
        elev->currFloor = elev->nextFloor; // Sets the current floor
                                           // to the next floor
        sem_wait(&pwMutex); // Protects people waiting on each floor
        if (peopleWaiting[elev->currFloor])
        { // if people are waiting, stop
            floor_stop(elev->currFloor);
            movement = 1;
        }
        sem_post(&pwMutex);

        // Did the elevator change direction?
        int changedDirection = change_direction(elev->direction,
                                                elev->currFloor);

        if (!changedDirection)
        { // if direction stayed the same
            if (elev->direction)
            { // elevator moving upwards
                elev->nextFloor = elev->currFloor + 1;
            }
            else
            { // elevator moving downwards
                elev->nextFloor = elev->currFloor - 1;
            }
        }
        else
        {
            if (elev->direction)
            { // if previous direction was upwards
                start_downward();
                elev->nextFloor = elev->currFloor - 1;
            }
            else
            {
                if (!movement)
                {
                    no_movement(finalTrip, elev->maxWaitTime);
                    finalTrip = 1;
                }
                else
                {
                    finalTrip = 0;
                }
                start_upward();
                elev->nextFloor = elev->currFloor + 1;
                movement = 0;
            }
            elev->direction = !elev->direction;
        }
    }
}
// This stops the elevator and "opens the doors" for the passenger
void floor_stop(int currFloor)
{
    sem_wait(&printMutex);
    printf("Elevator: Opening doors at %i\n", currFloor);
    sem_post(&printMutex);
    sem_post(&floorCall[currFloor]);
    sleep(1);
    sem_wait(&floorCall[currFloor]);
}

// This function checks if the direction of the elevator changed
int change_direction(int direction, int currFloor)
{
    int directionChanged = 0; // Value that will be returned.
                              // 0 for no, 1 for yes

    if ((direction && currFloor == numberOfFloors) ||
        (!direction && currFloor == 0))
    {
        directionChanged = 1;
    }

    return directionChanged;
}

// This prints the comment for the elevator starting its descent
void start_downward()
{
    sem_wait(&printMutex);
    printf("Elevator: Heading to min floor 0.\n");
    sem_post(&printMutex);
}

// At floor 0, this checks if the elevator should exit the program
void no_movement(int finalTrip, int sleepTime)
{
    if (finalTrip)
    {
        sem_wait(&printMutex);
        printf("Elevator: Leaving The System\n");
        sem_post(&printMutex);
        pthread_exit(0);
        return;
    }
    sem_wait(&printMutex);
    printf("Elevator: At min floor 0. No people found. ");
    printf("Going to sleep for %i seconds...\n", sleepTime);
    sem_post(&printMutex);
    sleep(sleepTime);
}

// Prints the comment for the elevator starting its climb
// Also prints the floors people want to go to
void start_upward()
{
    sem_wait(&printMutex);
    printf("Elevator: At floor 0\n");
    printf("Number of people waiting at floor\n");
    printf("Floor Number\tNumber of People\n");
    for (int i = 0; i <= numberOfFloors; i++)
    {
        printf("%i\t%i\n", i, peopleWaiting[i]);
    }
    printf("Elevator: Heading to max floor %i\n", numberOfFloors);
    sem_post(&printMutex);
}