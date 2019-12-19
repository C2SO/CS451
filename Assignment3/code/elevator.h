// Nicholas Rahbany
// CS451
// elevator.h

#define ELEVATOR

typedef struct elevator
{
    int maxWaitTime;
    int totalFloors;
    int direction;
    int currFloor;
    int nextFloor;
} elevator;

void* elevator_travel(void *args);
int change_direction(int direction, int currFloor);
void no_movement(int finalTrip, int sleepTime);
void start_upward();
void start_downward();
void floor_stop(int currFloor);