// Nicholas Rahbany
// CS451
// person.c

#include <stdio.h>
#include <pthread.h>
#include <unistd.h>
#include <semaphore.h>
#include <stdlib.h>
#include "sem.h"
#include "person.h"

void* enter_building(void* args);
void exit_elevator(int floor, int PID);
void enter_elevator(int floor, int PID);
void add_floor_to_queue(int floor);
void remove_floor_from_queue(int floor);
struct person* person_init(int index);

sem_t printMutex;
sem_t floorCall[256];
sem_t pwMutex;
int peopleWaiting[ARRAY_SIZE];

// This simulates the person's logic for their schedule
void* enter_building(void *args)
{
    struct person *passenger = (struct person *)args;
    int floorIndex, currFloor, nextFloor = 0;

    // Infinite loop until the user "exits" the building
    while (1)
    {
        nextFloor = passenger->floorSchedule[floorIndex];
        exit_elevator(nextFloor, passenger->PID);
        currFloor = nextFloor;

        // Print user action
        printf("Person %d: ", passenger->PID);
        printf("Wandering floor %i ", currFloor);
        printf("for %i seconds\n",
                passenger->waitSchedule[floorIndex]);
        sem_post(&printMutex);

        // Waits for the allocated "wandering" time
        sleep(passenger->waitSchedule[floorIndex]);

        // Checks if the user will exit the system
        if (!(floorIndex < passenger->floorsToVisit - 1))
        {
            printf("Person %d: Leaving the System Goodbye!\n",
                    passenger->PID);
            pthread_exit(0);
        }

        // Wait for another elevator
        enter_elevator(currFloor, passenger->PID);
        floorIndex++;
    }
}

// Called when a person exits the elevator
void exit_elevator(int floor, int PID)
{
    // Push the elevator button
    add_floor_to_queue(floor);

    // Print user action
    sem_wait(&printMutex);
    printf("Person %d: ", PID);
    printf("Waiting for elevator to reach floor %d\n", floor);
    sem_post(&printMutex);

    // Print the user getting off the elevator at the correct floor
    sem_wait(&floorCall[floor]);
    sem_wait(&printMutex);
    printf("Person %d: ", PID);
    printf("Getting off elevator on floor %d\n", floor);
    sem_post(&floorCall[floor]);

    // Remove the user from the queue
    remove_floor_from_queue(floor);
}

// Simulates a person waiting for an elevator
void enter_elevator(int floor, int PID)
{
    // User waiting for elevator
    add_floor_to_queue(floor);
    sem_wait(&printMutex);
    printf("Person %d: Waiting at floor %d\n", PID, floor);
    sem_post(&printMutex);

    // When the elevator arrives on the floor
    sem_wait(&floorCall[floor]);
    printf("Person %d: ", PID);
    printf("Getting on elevator at floor %d\n", floor);
    sem_post(&pwMutex);
    sem_post(&floorCall[floor]);

    // Remove the user from the queue
    remove_floor_from_queue(floor);
}

// Adds a person to the queue
void add_floor_to_queue(int floor)
{
    sem_wait(&pwMutex);
    peopleWaiting[floor]++;
    sem_post(&pwMutex);
}

// Removes a person from the queue
void remove_floor_from_queue(int floor)
{
    sem_wait(&pwMutex);
    peopleWaiting[floor]--;
    sem_post(&pwMutex);
}

// Initializes a person
struct person* person_init(int initPID)
{
    struct person* newPerson = (struct person *)malloc(sizeof(struct person));

    // Print a person's information
    sem_wait(&printMutex);
    printf("Person %d's schedule:\nFloor \tTime Spent\n", initPID);

    int floorsVisited;
    scanf("%i", &floorsVisited);

    newPerson->PID = initPID;
    newPerson->floorsToVisit = floorsVisited;

    // Store/print the user's schedule
    for (int i = 0; i < floorsVisited; i++)
    {
      scanf("%i", &newPerson->floorSchedule[i]);
      scanf("%i", &newPerson->waitSchedule[i]);

      printf("%i\t", newPerson->floorSchedule[i]);
      printf("%i\n", newPerson->waitSchedule[i]);
    }
    printf("\n");
    sem_post(&printMutex);
    
    return newPerson;
}