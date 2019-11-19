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
#define ALARM_MODE 1        // mode == alarm
#define STOP_WATCH_MODE 2   // mode == stopWatch

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
#define MODIFY_X 0
#define BUTTON_INPUT_X 125 - MODIFY_X
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
typedef struct TimeSet //time data
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
typedef struct AlarmData //AlarmData of digital watch
{
    int alarmIndicator;
    int *alarmState;
    int hour;
    int min;
} AlarmData;

typedef struct StateData //StateData of digital watch
{
    char mode;
    char innerMode;
    char timeKeepingUnit;
    char alarmUnit;
    TimeSet currentTime;
    TimeSet stopWatch;
    TimeSet lapTime;
} StateData;

typedef struct PassingData //Data for backlight & alarm controller
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
typedef struct ButtonData //Data for button input control
{
    char *inputButton;
    char *processedButton;
    int *buttonLength;
    int *buttonThreadBoundaryLine;
    int *mainBoundaryLine;
    int *alarmState;
} ButtonData;

void setDefault(StateData *, AlarmData *);              //Default set of digital watch
void printWatchManual(StateData *);                     //Print watch's manual for each mode
void timeProcess(StateData *sate, AlarmData *);         //Process Time flow of Time Keeping & StopWatch
void showWatch(StateData *, AlarmData *);               //Show watch based on StateData, Alarm Data
void decideMainProcess(StateData *, AlarmData *, char); //Decide main controller process based on state, button and alarm data
void mainProcessA(StateData *, AlarmData *, char);      //Main process for button 'A'
void mainProcessB(StateData *, AlarmData *, char);      //Main process for button 'B'
void mainProcessC(StateData *, AlarmData *, char);      //Main process for button 'C'
void *buttonThreadFunction(void *);                     //Thread for user input
void buttonProcess(char *, char *);                     //Process simultaneous input processing
void buttonInitialize(char *, int, int);                //Initialize button in range begin ~ MAX
void *alarmThreadFunction(void *);                      //Alarm controller thread
int isAlarmTime(StateData *, AlarmData *);              //Check It's alarm time
void *backlightThreadFunction(void *);                  //Backlight controller thread
void timeKeepingMode(StateData *);                      //Enable timeKeepingMode
void timeKeepingSet(StateData *);                       //Enable timeKeepingSet
void timeKeepingUnitChange(StateData *);                //Trigger timeKeeping unit change
void stopWatchMode(StateData *);                        //Enable stopWatchMode
void stopWatchLaptime(StateData *);                     //Trigger stop watch laptime
void stopWatchRun(StateData *);                         //Enable stop watch Run
void stopWatchPause(StateData *);                       //Enable stop watch Pause
void alarmMode(StateData *);                            //Enable alarmMode
void alarmSetMode(StateData *);                         //Enable alarmSetMode
void alarmIndicator(AlarmData *);                       //ON&OFF alarm indicator
void addValue(StateData *, AlarmData *);                //Add value of targeted unit
void selectionSort(char *);                             //For button processing
int getMaxIdx(char *, int);                             //Get max index of processed button for selection sort
int getButtonLength(char *);                            //Get processed button length
void gotoxy(int, int);                                  //Gotoxy
void textcolor(int, int);                               //For text color(backlight)
void modeChangePrint();                                 //Initialize console when mode changed
void printUnderLine(StateData, AlarmData);              //Print underline for targeted time unit
