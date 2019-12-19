// Nicholas Rahbany
// CS451
// sem.c

#include <stdio.h>
#include <semaphore.h>
#include "sem.h"

extern sem_t printMutex;
extern sem_t floorCall[ARRAY_SIZE];
extern sem_t pwMutex;
extern int numberOfFloors;

// Creates all semaphores
void sem_create_all()
{
    sem_init(&printMutex, 0, 1);
    for (int i = 0; i < 4; i++)
    {
        sem_init(&floorCall[i], 0, 0);
    }
    sem_init(&pwMutex, 0, 1);
}