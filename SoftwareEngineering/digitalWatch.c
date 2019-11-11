#include <stdio.h>
#include <pthread.h>
#include <string.h>
#define TRUE 1==1
#define FALSE 1!=1
#define ON 1
#define OFF 0
//struct State Data variable mode preset
#define TIME_KEEPING_MODE 1 // mode == timeKeeping
#define STOP_WATCH_MODE 2 // mode == stopWatch
#define ALARM_MODE 3 // mode == alarm

//struct State Data variable innerMode preset
#define DEFAULT 1 // inner mode == default
#define TIME_KEEPING_SET 2 // inner mode == timeKeeping->timeKeepingSet
#define STOP_WATCH_RUN 3 // stopWatch mode == stopWatch->stopWatchRun
#define STOP_WATCH_LAPTIME 4 // stopWatch mode == stopWatch->laptime
#define STOP_WATCH_PAUSE 5 // stopWatch mode == stopWatch->pause

//timeKeepingUnit preset
#define HOUR 1
#define MIN 2
#define SEC 3
#define MILLI_SEC 4

typedef struct AlarmData{
    int indicator;
    int hour;
    int min;
}AlarmData;

typedef struct StateData{
    char mode;
    char innerMode;
    char timeKeepingUnit;
    char alarmUnit;
}StateData;

void* thread1(void*);
void* thread2(void*);
void timeKeepingMode(StateData*);
void timeKeepingSet(StateData*);
void timeKeepingUnitChange(StateData*);
void stopWatchMode(StateData*);
void alarmMode(StateData*);
void showWatch(StateData*);
int main(void)
{ 
    char* weekOfDays[7] = 
    {
        "MO",
        "TU",
        "TH",
        "WE",
        "FR",
        "SA",
        "SU",
    };
    char button;
    pthread_t Alarm;
    pthread_t BackLight;
    // pthread_create(&Alarm, NULL, thread1, void*);
    // pthread_create(&BackLight, NULL, thread2, void*);
    pthread_join(&Alarm, NULL);
    pthread_join(&BackLight, NULL);
    // pthread_create()
    return 0;
}

void* thread1(void* test)
{

}

void* thread2(void* test)
{

}

void timeKeepingMode(StateData* state) // enable timeKeepingMode
{
    state->mode = TIME_KEEPING_MODE;
    state->innerMode = DEFAULT;
}

void timeKeepingSet(StateData* state) // enable timeKeepingSet
{
    if(state->mode == TIME_KEEPING_MODE)
        state->innerMode = TIME_KEEPING_SET;
}

void timeKeepingUnitChange(StateData* state)
{
    if(state->mode!=TIME_KEEPING_MODE)
        return;
    // state->timeKeepingUnit % 
}
void stopWatchMode(StateData* state)
{
    state->mode = STOP_WATCH_MODE;
    state->innerMode = DEFAULT;
}

void alarmMode(StateData* state)
{
    state->mode = ALARM_MODE;
    state->innerMode = DEFAULT;
}

void showWatch(StateData* state) // show watch based on StateData
{
    if(state->mode == TIME_KEEPING_MODE) // Time Keeping Mode
    {
        printf("%02d:");
    }
    else if(state->mode == STOP_WATCH_MODE) // Stop Watch Mode
    {

    }
    else // Alarm Mode
    {

    }
}