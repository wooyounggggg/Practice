#include <stdio.h>
#include <pthread.h>
#include <string.h>
#include <time.h>
#define TRUE 1 == 1
#define FALSE 1 != 1
#define ON 1
#define OFF 0
//struct State Data variable mode preset
#define TIME_KEEPING_MODE 1 // mode == timeKeeping
#define STOP_WATCH_MODE 2   // mode == stopWatch
#define ALARM_MODE 3        // mode == alarm

//struct State Data variable innerMode preset
#define DEFAULT 1            // inner mode == default
#define TIME_KEEPING_SET 2   // inner mode == timeKeeping->timeKeepingSet
#define STOP_WATCH_RUN 3     // stopWatch mode == stopWatch->stopWatchRun
#define STOP_WATCH_LAPTIME 4 // stopWatch mode == stopWatch->laptime
#define STOP_WATCH_PAUSE 5   // stopWatch mode == stopWatch->pause

//timeKeepingUnit preset
#define NONE -1
#define SEC 0
#define HOUR 1
#define MIN 2
#define YEAR 3
#define MONTH 4
#define DAY 5

//alarmUnit preset
#define NONE -1
#define HOUR 0
#define MIN 1

#define INPUT_BUTTON_SIZE 100
#define PROCESSED_BUTTON_SIZE 5
typedef struct AlarmData
{
    int alarmIndicator;
    int alarmState;
    int hour;
    int min;
} AlarmData;

typedef struct PassingData //Data to backlight & alarm controller
{
    AlarmData *alarm;
    char button;
} PassingData;
typedef struct StateData
{
    char mode;
    char innerMode;
    char timeKeepingUnit;
    char alarmUnit;
} StateData;

void setDefault(StateData *, AlarmData *);
void decideMainProcess(StateData *, AlarmData *, char); //Decide main controller process based on state, button and alarm data
void buttonInitialize(char *, int, int);                //Initialize button in range begin ~ MAX
void *alarmThreadFunction(void *);                      //Alarm controller thread
void *backlightThreadFunction(void *);                  //Backlight controller thread
void buttonProcess(char *, char *);                     //Process simultaneous input processing
void timeKeepingMode(StateData *);                      // Enable timeKeepingMode
void timeKeepingSet(StateData *);                       // Enable timeKeepingSet
void timeKeepingUnitChange(StateData *);                // Trigger timeKeeping unit change
void stopWatchMode(StateData *);                        // Enable stopWatchMode
void alarmMode(StateData *);                            // Enable alarmMode
void showWatch(StateData *);                            // show watch based on StateData

int main(void)
{
    char *weekOfDays[7] =
        {
            "MO",
            "TU",
            "TH",
            "WE",
            "FR",
            "SA",
            "SU",
        };
    char inputButton[INPUT_BUTTON_SIZE] = {0};
    char processedButton[PROCESSED_BUTTON_SIZE];
    StateData state;
    AlarmData alarm;
    PassingData pass;
    pthread_t alarmThread;
    pthread_t backlightThread;
    int i;
    int alarmThreadCounter = 0;
    int backlightThreadCounter = 0;
    setDefault(&state, &alarm); //state, alarm initialize
    pass.alarm = &alarm;
    while (1)
    {
        printf("Button : ");
        scanf("%s", inputButton);
        buttonProcess(inputButton, processedButton);
        for (i = 0; i < PROCESSED_BUTTON_SIZE; i++)
        {
            if (processedButton[i] == 0 || !(processedButton[i] >= 'A' && processedButton[i] <= 'D'))
                continue;
            pass.button = processedButton[i];
            if (alarmThreadCounter == 0)
            {
                pthread_create(&alarmThread, NULL, &alarmThreadFunction, (void *)&pass);
                alarmThreadCounter++;
            }
            if (backlightThreadCounter == 0)
            {
                pthread_create(&backlightThread, NULL, &backlightThreadFunction, (void *)&pass);
                backlightThreadCounter++;
            }
            decideMainProcess(&state, &alarm, processedButton[i]);
            if (alarmThreadCounter == 1)
            {
                pthread_join(alarmThread, NULL);
                alarmThreadCounter--;
            }
            if (backlightThreadCounter == 1)
            {
                pthread_join(backlightThread, NULL);
                backlightThreadCounter--;
            }
        }
        buttonInitialize(inputButton, 0, INPUT_BUTTON_SIZE);
        buttonInitialize(processedButton, 0, PROCESSED_BUTTON_SIZE);
    }
    // pthread_join(&Alarm, NULL);
    // pthread_join(&BackLight, NULL);
    return 0;
}

void setDefault(StateData *state, AlarmData *alarm)
{
    state->mode == TIME_KEEPING_MODE;
    state->innerMode = DEFAULT;
    state->timeKeepingUnit = NONE;
    state->alarmUnit = NONE;
    //state's time struct initialize
    alarm->alarmIndicator = OFF;
    alarm->alarmState = OFF;
    alarm->hour = NONE;
    alarm->min = NONE;
}
void *alarmThreadFunction(void *_pass) //Alarm controller thread
{
    PassingData *pass = (PassingData *)_pass;
}

void *backlightThreadFunction(void *_pass) //Backlight controller thread
{
    PassingData *pass = (PassingData *)_pass;
}
void decideMainProcess(StateData *state, AlarmData *alarm, char button) //Decide main controller process based on state, button and alarm data
{
    printf("%c\n", button);
}
void buttonInitialize(char *button, int begin, int size) //Initialize button in range begin ~ MAX
{
    int i;
    for (i = begin; i < size; i++)
    {
        button[i] = 0;
    }
}
void buttonProcess(char *inputButton, char *processedButton) //Process simultaneous input processing
{
    int i;
    int checkButton = 0;
    // char checkChar[4] = {0};
    for (i = 0; i < INPUT_BUTTON_SIZE; i++)
    {
        if (inputButton[i] == 'A' || inputButton[i] == 'a')
            processedButton[3] = 'A';
        else if (inputButton[i] == 'B' || inputButton[i] == 'b')
            processedButton[2] = 'B';
        else if (inputButton[i] == 'C' || inputButton[i] == 'c')
            processedButton[1] = 'C';
        else if (inputButton[i] == 'D' || inputButton[i] == 'd')
            processedButton[0] = 'D';
    }
    buttonInitialize(inputButton, 0, INPUT_BUTTON_SIZE);
}
void timeKeepingMode(StateData *state) // Enable timeKeepingMode
{
    state->mode = TIME_KEEPING_MODE;
    state->innerMode = DEFAULT;
}

void timeKeepingSet(StateData *state) // Enable timeKeepingSet
{
    if (state->mode == TIME_KEEPING_MODE)
        state->innerMode = TIME_KEEPING_SET;
}

void timeKeepingUnitChange(StateData *state) // Trigger timeKeeping unit change
{
    if (state->mode != TIME_KEEPING_MODE)
        return;
    // state->timeKeepingUnit %
}
void stopWatchMode(StateData *state) // Enable stopWatchMode
{
    state->mode = STOP_WATCH_MODE;
    state->innerMode = DEFAULT;
}

void alarmMode(StateData *state) // Enable alarmMode
{
    state->mode = ALARM_MODE;
    state->innerMode = DEFAULT;
}

void showWatch(StateData *state) // show watch based on StateData
{
    if (state->mode == TIME_KEEPING_MODE) // Time Keeping Mode
    {
        printf("%02d:");
    }
    else if (state->mode == STOP_WATCH_MODE) // Stop Watch Mode
    {
    }
    else // Alarm Mode
    {
    }
}