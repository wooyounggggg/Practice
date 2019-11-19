#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <string.h>
#include <time.h>
#include <windows.h>
#define ON 1
#define OFF 0
//struct State Data variable mode preset
#define TIME_KEEPING_MODE 0 // mode == timeKeeping
#define STOP_WATCH_MODE 1   // mode == stopWatch
#define ALARM_MODE 2        // mode == alarm

//struct State Data variable innerMode preset
#define DEFAULT 0            // inner mode == default
#define TIME_KEEPING_SET 1   // inner mode == timeKeeping->timeKeepingSet
#define STOP_WATCH_RUN 2     // stopWatch mode == stopWatch->stopWatchRun
#define STOP_WATCH_LAPTIME 3 // stopWatch mode == stopWatch->laptime
#define STOP_WATCH_PAUSE 4   // stopWatch mode == stopWatch->pause
#define ALARM_SET 5

//struct State Data timeKeepingUnit preset
#define NONE -1
#define TIME_KEEPING_SEC 0
#define TIME_KEEPING_HOUR 1
#define TIME_KEEPING_MIN 2
#define TIME_KEEPING_YEAR 3
#define TIME_KEEPING_MONTH 4
#define TIME_KEEPING_DAY 5

//struct Alarm Data alarmUnit preset
#define ALARM_HOUR 0
#define ALARM_MIN 1

//for text color print
#define YELLOW "\x1b[33m"
#define WHITE "\x1b[37m"

//for button input
#define INPUT_BUTTON_SIZE 100
#define PROCESSED_BUTTON_SIZE 5

//for thread synchronization
#define CLEAR 2
#define UNLOCK 1
#define LOCK 0

//for gotoxy
#define MODIFY_X 35
#define BUTTON_INPUT_X 125
#define BUTTON_INPUT_Y 25
#define TIME_LINE_X BUTTON_INPUT_X + 5
#define TIME_LINE_Y BUTTON_INPUT_Y - 5
#define DATE_LINE_X TIME_LINE_X - 4
#define DATE_LINE_Y TIME_LINE_Y - 2
#define MODE_LINE_X BUTTON_INPUT_X - 8
#define MODE_LINE_Y BUTTON_INPUT_Y + 2

//for sleep
#define TIME_KEEPING_SLEEP 10
#define STOP_WATCH_SLEEP 1
#define ALARM_SLEEP 10
typedef struct TimeSet
{
    int year;
    int month;
    int day;
    int hour;
    int min;
    int sec;
    int msec;
    int dayOfWeek;
} TimeSet;
typedef struct AlarmData
{
    int alarmIndicator;
    int *alarmState;
    int hour;
    int min;
} AlarmData;

typedef struct StateData
{
    char mode;
    char innerMode;
    char timeKeepingUnit;
    char alarmUnit;
    TimeSet currentTime;
    TimeSet stopWatch;
    TimeSet lapTime;
} StateData;

typedef struct PassingData //Data to backlight & alarm controller
{
    AlarmData *alarm;
    char *button;
    char *processedButton;
    struct tm *backlightTime;
    struct tm *alarmTime;
    int *backlightThreadBoundaryLine;
    int *alarmThreadBoundaryLine;
    int *mainBoundaryLine;
    int *backlightThreadCounter;
    int *alarmThreadCounter;
} PassingData;
typedef struct ButtonData
{
    char *inputButton;
    char *processedButton;
    int *buttonLength;
    int *buttonThreadBoundaryLine;
    int *mainBoundaryLine;
    int *alarmState;
    // char *currentButton;
} ButtonData;

void setDefault(StateData *, AlarmData *);
void decideMainProcess(StateData *, AlarmData *, char); //Decide main controller process based on state, button and alarm data
void mainProcessA(StateData *, AlarmData *, char);
void mainProcessB(StateData *, AlarmData *, char);
void mainProcessC(StateData *, AlarmData *, char);
void buttonInitialize(char *, int, int); //Initialize button in range begin ~ MAX
void printModeManual(StateData *);
void *alarmThreadFunction(void *);     //Alarm controller thread
void *backlightThreadFunction(void *); //Backlight controller thread
void *buttonThreadFunction(void *);
void buttonProcess(char *, char *);      //Process simultaneous input processing
void timeKeepingMode(StateData *);       // Enable timeKeepingMode
void timeKeepingSet(StateData *);        // Enable timeKeepingSet
void timeKeepingUnitChange(StateData *); // Trigger timeKeeping unit change
void stopWatchMode(StateData *);         // Enable stopWatchMode
void stopWatchLaptime(StateData *);
void stopWatchRun(StateData *);
void stopWatchPause(StateData *);
void alarmMode(StateData *); // Enable alarmMode
void alarmSetMode(StateData *);
void alarmIndicator(AlarmData *);
void addValue(StateData *, AlarmData *);
void timeProcess(StateData *sate, AlarmData *);
void showWatch(StateData *, AlarmData *); // show watch based on StateData
void selectionSort(char *);
int getMaxIdx(char *, int);
int getButtonLength(char *);
void gotoxy(int, int);
void textcolor(int, int);
void modeChangePrint();
int isAlarmTime(StateData *, AlarmData *);
void printUnderLine(StateData);