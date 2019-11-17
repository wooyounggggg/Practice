#include "digitalWatch.h"

void *backlightThreadFunction(void *_pass) //Backlight controller thread. need to pass tm data
{
    PassingData *pass = (PassingData *)_pass;
    printf(YELLOW);
    *(pass->backlightThreadCounter) = *(pass->backlightThreadCounter) + 1;
    Sleep(2000); //tick 2seconds.
    *(pass->backlightThreadCounter) = *(pass->backlightThreadCounter) - 1;
    if (*(pass->backlightThreadCounter) == 0)
        printf(WHITE);
}

void setDefault(StateData *state, AlarmData *alarm) //
{
    state->mode = TIME_KEEPING_MODE; //TIME_KEEPING_MODE;
    state->innerMode = DEFAULT;
    state->timeKeepingUnit = NONE;
    state->alarmUnit = NONE;
    //state's time struct initialize
    alarm->alarmIndicator = OFF;
    alarm->alarmState = OFF;
    alarm->hour = NONE;
    alarm->min = NONE;
}
void decideMainProcess(StateData *state, AlarmData *alarm, char button) //Decide main controller process based on state, button and alarm data
{
    if (alarm->alarmState == ON)
        return;
    switch (button)
    {
    case 'A':
        mainProcessA(state, alarm, button);
        break;
    case 'B':
        mainProcessB(state, alarm, button);
        break;
    case 'C':
        mainProcessC(state, alarm, button);
        break;
    case 'D':
        break;
    default:
        // printf("exception\n");
        break;
    }
    // gotoxy(40,40);
    // printf("%c\n", button);
}
void mainProcessA(StateData *state, AlarmData *alarm, char button) // main process for button 'A'
{
    printf("\nProcess A\n");
    if (state->mode == TIME_KEEPING_MODE)
    {
        if (state->innerMode == DEFAULT)
            timeKeepingSet(state);
        else if (state->innerMode == TIME_KEEPING_SET)
            timeKeepingMode(state);
    }
    else if (state->mode == STOP_WATCH_MODE)
    {
        if (state->innerMode == STOP_WATCH_LAPTIME || state->innerMode == STOP_WATCH_RUN)
            stopWatchLaptime(state);
        else if (state->innerMode == STOP_WATCH_PAUSE)
            stopWatchMode(state);
    }
    else if (state->mode == ALARM_MODE)
    {
        if (state->innerMode == DEFAULT)
            alarmSetMode(state);
        else if (state->innerMode == ALARM_SET)
            alarmMode(state);
    }
    printf("mode : %d, inner mode : %d indicator : %d\n", state->mode, state->innerMode, alarm->alarmIndicator);
    printf("\n");
}
void mainProcessB(StateData *state, AlarmData *alarm, char button) // main process for button 'B'
{
    printf("Process B\n");
    if (state->mode == TIME_KEEPING_MODE)
    {
        if (state->innerMode == TIME_KEEPING_SET && state->timeKeepingUnit != NONE)
            addValue(state, alarm);
    }
    else if (state->mode == STOP_WATCH_MODE)
    {
        if (state->innerMode == DEFAULT)
            stopWatchRun(state);
        else if (state->innerMode == STOP_WATCH_RUN)
            stopWatchPause(state);
        else if (state->innerMode == STOP_WATCH_PAUSE)
            stopWatchRun(state);
        else if (state->innerMode == STOP_WATCH_LAPTIME)
            stopWatchRun(state);
    }
    else if (state->mode == ALARM_MODE)
    {
        if (state->innerMode == ALARM_SET && state->alarmUnit != NONE)
            addValue(state, alarm);
        else if (state->innerMode == DEFAULT)
            alarmIndicator(alarm);
    }
    printf("mode : %d, inner mode : %d indicator : %d\n", state->mode, state->innerMode, alarm->alarmIndicator);
    printf("\n");
}
void mainProcessC(StateData *state, AlarmData *alarm, char button) // main process for button 'C'
{
    printf("Process C\n");
    if (state->innerMode == DEFAULT) //mode change
        state->mode = (state->mode + 1) % 3;
    else if (state->mode == TIME_KEEPING_MODE)
        state->timeKeepingUnit = (state->timeKeepingUnit + 1) % 6;

    else if (state->mode == ALARM_MODE)
        state->alarmUnit = (state->alarmUnit + 1) % 2;
    printf("mode : %d, inner mode : %d indicator : %d\n", state->mode, state->innerMode, alarm->alarmIndicator);
    printf("\n");
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
    int j;
    int max;
    int checkButton = 0;
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
    selectionSort(processedButton);
    buttonInitialize(inputButton, 0, INPUT_BUTTON_SIZE);
}
void timeKeepingMode(StateData *state) // Enable timeKeepingMode
{
    state->mode = TIME_KEEPING_MODE;
    state->innerMode = DEFAULT;
    state->timeKeepingUnit = NONE;
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
    state->timeKeepingUnit = (state->timeKeepingUnit + 1) % (TIME_KEEPING_DAY + 1);
}
void stopWatchMode(StateData *state) // Enable stopWatchMode
{
    state->mode = STOP_WATCH_MODE;
    state->innerMode = DEFAULT;
}

void stopWatchRun(StateData *state)
{
    state->innerMode = STOP_WATCH_RUN;
}
void stopWatchLaptime(StateData *state)
{
    if (state->mode == STOP_WATCH_MODE)
        state->innerMode = STOP_WATCH_LAPTIME;
    //initialize laptime
}
void stopWatchPause(StateData *state)
{
    state->innerMode = STOP_WATCH_PAUSE;
}

void alarmMode(StateData *state) // Enable alarmMode
{
    state->mode = ALARM_MODE;
    state->innerMode = DEFAULT;
}

void alarmSetMode(StateData *state)
{
    state->innerMode = ALARM_SET;
}

void alarmIndicator(AlarmData *alarm)
{
    alarm->alarmIndicator = !(alarm->alarmIndicator);
}

void addValue(StateData *state, AlarmData *alarm)
{
    int dayOfMonth = 30;
    if (state->mode == TIME_KEEPING_MODE)
        switch (state->timeKeepingUnit)
        {
        case TIME_KEEPING_SEC:
            state->currentTime.sec = (state->currentTime.sec + 1) % 60;
            break;
        case TIME_KEEPING_HOUR:
            state->currentTime.hour = (state->currentTime.hour + 1) % 24;
            break;
        case TIME_KEEPING_MIN:
            state->currentTime.min = (state->currentTime.min + 1) % 60;
            break;
        case TIME_KEEPING_YEAR:
            state->currentTime.year = state->currentTime.year + 1;
            break;
        case TIME_KEEPING_MONTH:
            state->currentTime.month = (state->currentTime.month + 1) % 12 + 1;
            break;
        case TIME_KEEPING_DAY:
            if ((state->currentTime.month <= 7 && state->currentTime.month % 2) ||
                (state->currentTime.month >= 8 && !(state->currentTime.month % 2))) // 31 or 31
                dayOfMonth = 31;
            state->currentTime.day = (state->currentTime.day + 1) % dayOfMonth + 1;
            break;
        default:
            break;
        }
    else
        switch (state->alarmUnit)
        {
        case ALARM_HOUR:
            alarm->hour = (alarm->hour + 1) % 24;
            break;
        case ALARM_MIN:
            alarm->min = (alarm->min + 1) % 60;
        default:
            break;
        }
}
void gotoxy(int x, int y)
{
    printf("\033[%d;%df", y, x);
    fflush(stdout);
}

int getButtonLength(char *processedButton)
{
    int i;
    int length = 0;
    for (i = 0; i < 4; i++)
    {
        if (processedButton[i] > 'D' || processedButton[i] < 'A')
            break;
        length++;
    }
    return length;
}
void selectionSort(char *processedButton)
{
    int i;
    char tmp;
    for (i = 0; i < PROCESSED_BUTTON_SIZE; i++)
    {
        int maxIdx = getMaxIdx(processedButton, i);
        tmp = processedButton[i];
        processedButton[i] = processedButton[maxIdx];
        processedButton[maxIdx] = tmp;
    }
}
int getMaxIdx(char *processedButton, int begin) //get max index of processed button for selection sort
{
    int i, j;
    int maxIdx = begin;
    for (i = begin; i < PROCESSED_BUTTON_SIZE - 1; i++)
    {
        if (processedButton[maxIdx] < processedButton[i])
            maxIdx = i;
    }
    return maxIdx;
}

void textcolor(int foreground, int background)
{
    int color = foreground + background * 16;
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), color);
}