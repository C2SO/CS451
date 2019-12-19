// Nicholas Rahbany
// CS451
// sem.h

#define SEM
#include <semaphore.h>

#define ARRAY_SIZE 256

extern sem_t printMutex;
extern sem_t floorCall[ARRAY_SIZE];
extern sem_t pwMutex;

void sem_create_all();