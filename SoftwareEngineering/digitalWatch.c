#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <string.h>
#include <time.h>
#include <windows.h>
#include "digitalWatch.h"
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
    char inputButton[INPUT_BUTTON_SIZE] = {0};   //User's button input. It will be processed and stored at processed button.
    char processedButton[PROCESSED_BUTTON_SIZE]; //processed 4(random A~D -> D-C-B-A) button from inputButton.
    char currentButton;
    StateData state;
    AlarmData alarm;
    PassingData pass;
    ButtonData button;
    timer_t timer;
    struct tm *currentTime;
    struct tm *backlightTime;
    struct tm *alarmTime;
    struct tm modifyTimeValue;
    pthread_t alarmThread;
    pthread_t backlightThread;
    pthread_t buttonThread;
    int i;
    int buttonLength;
    int backlightThreadBoundaryLine;
    int alarmThreadBoundaryLine;
    int buttonThreadBoundaryLine;
    int mainBoundaryLine;
    setDefault(&state, &alarm); //state, alarm initialize
    currentTime = localtime(&timer);
    pass.alarm = &alarm;
    pass.backlightThreadBoundaryLine = &backlightThreadBoundaryLine;
    pass.alarmThreadBoundaryLine = &alarmThreadBoundaryLine;
    pass.mainBoundaryLine = &mainBoundaryLine;

    button.inputButton = inputButton;
    button.processedButton = processedButton;
    button.buttonLength = &buttonLength;
    button.buttonThreadBoundaryLine = &buttonThreadBoundaryLine;
    mainBoundaryLine = UNLOCK;
    pthread_create(&alarmThread, NULL, &alarmThreadFunction, (void *)&pass);
    pthread_create(&backlightThread, NULL, &backlightThreadFunction, (void *)&pass);
    pthread_create(&buttonThread, NULL, &buttonThreadFunction, (void *)&button);
    while (1)
    {
        // system("clear");
        // gotoxy(50, 50);
        // int buttonLength;
        // printf(WHITE);
        // printf("Button : ");
        // scanf("%s", inputButton);
        // buttonProcess(inputButton, processedButton);
        // buttonLength = getButtonLength(processedButton);
        // printf("Processed button length : %d\n", buttonLength);
        mainBoundaryLine = LOCK;
        if (*(button.buttonThreadBoundaryLine) == UNLOCK)
        {
            for (i = 0; i < *(button.buttonLength); i++)
            {
                currentButton = processedButton[i];
                pass.button = &currentButton;
                decideMainProcess(&state, &alarm, &modifyTimeValue, currentButton);
            }
            *(button.buttonThreadBoundaryLine) = LOCK;
        }
        // printf("test\n");
        mainBoundaryLine = UNLOCK;
        // if (*(button.buttonThreadBoundaryLine) == OFF)
        // {
        //     buttonInitialize(inputButton, 0, INPUT_BUTTON_SIZE);
        //     buttonInitialize(processedButton, 0, PROCESSED_BUTTON_SIZE);
        // }
    }
    pthread_join(alarmThread, NULL);
    pthread_join(backlightThread, NULL);
    pthread_join(buttonThread, NULL);
    return 0;
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
void *alarmThreadFunction(void *_pass) //Alarm controller thread. need to pass tm data
{
    PassingData *pass = (PassingData *)_pass;
    // (*(pass->alarmThreadCounter))--;
    //need to have tm data
}

void *backlightThreadFunction(void *_pass) //Backlight controller thread. need to pass tm data
{
    PassingData *pass = (PassingData *)_pass;
    timer_t timerBefore;
    timer_t timerAfter;
    struct tm *currentTime /* = localtime(&timerBefore)*/;
    int backlight = OFF;
    while (1)
    {
        if (backlight == OFF)
        {
            if (pass->button == NULL || pass->alarm->alarmState == ON)
                continue;
            else if (*(pass->button) == 'D')
            {
                backlight = ON;
                timerBefore = time(NULL);
                currentTime = localtime(&timerBefore);
                printf(YELLOW);
                // printf("%c\n", *(pass->button));
                if (*(pass->button) == 'D')
                    *(pass->button) = 0;
            }
        }
        else if (backlight == ON)
        {
            if (timeDifference(timerBefore, timerAfter, 2))
            {
                printf(WHITE);
                backlight = OFF;
            }
            // printf("%c\n", *(pass->button));
            if (*(pass->button) == 'D')
                *(pass->button) = 0;
            // printf("%c\n",*(pass->button));
        }

        // Sleep(2000); //tick 2seconds.
        // printf(WHITE);
        // (*(pass->backlightThreadCounter))--;
    } //need to have tm data
}

void *buttonThreadFunction(void *_buttonList)
{
    ButtonData *buttonList = _buttonList;
    while (1)
    {
        // *(buttonList->buttonThreadBoundaryLine) = ON;
        printf("Button : ");
        scanf("%s", buttonList->inputButton);
        buttonProcess(buttonList->inputButton, buttonList->processedButton);
        *(buttonList->buttonThreadBoundaryLine) = UNLOCK;
        *(buttonList->buttonLength) = getButtonLength(buttonList->processedButton);
        if (*(buttonList->mainBoundaryLine) == UNLOCK)
        {
            buttonInitialize(buttonList->inputButton, 0, INPUT_BUTTON_SIZE);
            buttonInitialize(buttonList->processedButton, 0, PROCESSED_BUTTON_SIZE);
            *(buttonList->mainBoundaryLine) = LOCK;
        }
        // printf("Processed button length : %d\n", *(buttonList->buttonLength));
    }
}
int timeDifference(timer_t before, timer_t after, int sec)
{
    after = time(NULL);
    // printf("after - before : %d\n", after-before);
    Sleep(1000);
    return after - before >= sec;
}
void decideMainProcess(StateData *state, AlarmData *alarm, struct tm *modifyTimeValue, char button) //Decide main controller process based on state, button and alarm data
{
    if (alarm->alarmState == ON)
        return;
    switch (button)
    {
    case 'A':
        mainProcessA(state, alarm, button);
        break;
    case 'B':
        mainProcessB(state, alarm, modifyTimeValue, button);
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
    else
        printf("exception : %d\n", state->mode);
    printf("mode : %d, inner mode : %d indicator : %d\n", state->mode, state->innerMode, alarm->alarmIndicator);
    printf("\n");
}
void mainProcessB(StateData *state, AlarmData *alarm, struct tm *modifyTimeValue, char button) // main process for button 'B'
{
    printf("Process B\n");
    if (state->mode == TIME_KEEPING_MODE)
    {
        if (state->innerMode == TIME_KEEPING_SET && state->timeKeepingUnit != NONE)
            addValue(state, alarm, modifyTimeValue);
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
            addValue(state, alarm, modifyTimeValue);
        else if (state->innerMode == DEFAULT)
            alarmIndicator(alarm);
    }
    else
        printf("exception : %d\n", state->mode);
    printf("mode : %d, inner mode : %d indicator : %d\n", state->mode, state->innerMode, alarm->alarmIndicator);
    printf("\n");
}
void mainProcessC(StateData *state, AlarmData *alarm, char button) // main process for button 'C'
{
    printf("Process C\n");
    if (state->innerMode == DEFAULT) //mode change
        state->mode = (state->mode + 1) % 3;
    else if (state->mode == TIME_KEEPING_MODE)
        //    if(state->innerMode == TIME_KEEPING_SET) //time keeping unit change
        state->timeKeepingUnit = (state->timeKeepingUnit + 1) % 6;

    else if (state->mode == ALARM_MODE)
        // if(state->innerMode == ALARM_SET)
        state->alarmUnit = (state->alarmUnit + 1) % 2;
    else
        printf("exception : %d\n", state->mode);

    printf("mode : %d, inner mode : %d indicator : %d\n", state->mode, state->innerMode, alarm->alarmIndicator);
    printf("\n");
}
/* nothing in main for button 'D'*/
// void mainProcessD(StateData *state, AlarmData *alarm, char button) // main process for button 'D'
// {
//     printf("Process D\n");
//     if (state->mode == TIME_KEEPING_MODE)
//     {
//     }
//     else if (state->mode == STOP_WATCH_MODE)
//     {
//     }
//     else if (state->mode == ALARM_MODE)
//     {
//     }
//     else
//         printf("exception : %d\n", state->mode);
//     printf("\n");
// }
void buttonInitialize(char *button, int begin, int size) //Initialize button in range begin ~ MAX
{
    int i;
    for (i = begin; i < size; i++)
        button[i] = 0;
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

void addValue(StateData *state, AlarmData *alarm, struct tm *modifyTimeValue)
{
    if (state->mode == TIME_KEEPING_MODE)
        switch (state->timeKeepingUnit)
        {
        case TIME_KEEPING_SEC:
            modifyTimeValue->tm_sec = modifyTimeValue->tm_sec + 1;
            break;
        case TIME_KEEPING_HOUR:
            modifyTimeValue->tm_hour = modifyTimeValue->tm_hour + 1;
            break;
        case TIME_KEEPING_MIN:
            modifyTimeValue->tm_min = modifyTimeValue->tm_min + 1;
            break;
        case TIME_KEEPING_YEAR:
            modifyTimeValue->tm_year = modifyTimeValue->tm_year + 1;
            break;
        case TIME_KEEPING_MONTH:
            modifyTimeValue->tm_mon = modifyTimeValue->tm_mon + 1;
            break;
        case TIME_KEEPING_DAY:
            modifyTimeValue->tm_mday = modifyTimeValue->tm_mday + 1;
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

void setCurrentTime()
{
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