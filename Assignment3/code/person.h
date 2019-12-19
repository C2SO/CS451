// Nicholas Rahbany
// CS451
// person.h

#define PERSON

extern int peopleWaiting[ARRAY_SIZE];

typedef struct person
{
    int PID;
    int floorsToVisit;
    int floorSchedule[ARRAY_SIZE];
    int waitSchedule[ARRAY_SIZE];
} person;

void* enter_building(void* args);
void exit_elevator(int floor, int PID);
void enter_elevator(int floor, int PID);
void add_floor_to_queue(int floor);
void remove_floor_from_queue(int floor);
struct person* person_init(int index);