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
#define NONE -1
#define ALARM_HOUR 0
#define ALARM_MIN 1

//for text color print
#define YELLOW "\x1b[33m"
#define WHITE "\x1b[37m"

#define INPUT_BUTTON_SIZE 100
#define PROCESSED_BUTTON_SIZE 5

#define UNLOCK 1
#define LOCK 0
typedef struct AlarmData
{
    int alarmIndicator;
    int alarmState;
    int hour;
    int min;
} AlarmData;

typedef struct StateData
{
    char mode;
    char innerMode;
    char timeKeepingUnit;
    char alarmUnit;
    //need time struct for alarm, backlight
} StateData;

typedef struct PassingData //Data to backlight & alarm controller
{
    AlarmData *alarm;
    char *button;
    struct tm *backlightTime;
    struct tm *alarmTime;
    int *backlightThreadBoundaryLine;
    int *alarmThreadBoundaryLine;
    int *mainBoundaryLine;
    // int *alarmThreadCounter;
    // int *backlightThreadCounter;
} PassingData;
typedef struct ButtonData
{
    char *inputButton;
    char *processedButton;
    int *buttonLength;
    int *buttonThreadBoundaryLine;
    int *mainBoundaryLine;
    // char *currentButton;
} ButtonData;

void setDefault(StateData *, AlarmData *);
void decideMainProcess(StateData *, AlarmData *, struct tm *, char); //Decide main controller process based on state, button and alarm data
void mainProcessA(StateData *, AlarmData *, char);
void mainProcessB(StateData *, AlarmData *, struct tm *, char);
void mainProcessC(StateData *, AlarmData *, char);
void buttonInitialize(char *, int, int); //Initialize button in range begin ~ MAX
void *alarmThreadFunction(void *);       //Alarm controller thread
void *backlightThreadFunction(void *);   //Backlight controller thread
void *buttonThreadFunction(void *);
int timeDifference(timer_t, timer_t, int);
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
void addValue(StateData *, AlarmData *, struct tm *);
void showWatch(StateData *); // show watch based on StateData
void selectionSort(char *);
int getMaxIdx(char *, int);
int getButtonLength(char *);
void gotoxy(int, int);
void textcolor(int, int);

// pthread_mutex_t mutex_lock;