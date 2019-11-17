#include "digitalWatch.h"
void printModeManual(StateData *state)
{
    gotoxy(MODE_LINE_X, MODE_LINE_Y);
    if (state->mode == TIME_KEEPING_MODE)
    {
        if (state->innerMode == DEFAULT)
            printf("A : 시간 설정  C : 스탑워치");
        else if (state->innerMode = TIME_KEEPING_SET)
        {
            gotoxy(MODE_LINE_X - 4, MODE_LINE_Y);
            printf("A : 설정 완료  B : 시간 증가  C : 설정 대상 변경");
        }
    }
    else if (state->mode == STOP_WATCH_MODE)
    {
        if (state->innerMode == DEFAULT)
            printf("B : 스탑워치 시작  C : 알람");
        else if (state->innerMode == STOP_WATCH_RUN)
            printf("A : 기록  B : 스탑워치 멈춤");
        else if (state->innerMode == STOP_WATCH_LAPTIME)
            printf("A : 기록 갱신  B : 스탑워치 보기");
        else if (state->innerMode == STOP_WATCH_PAUSE)
            printf("A : 스탑워치 초기화  B : 스탑워치 재시작");
    }
    else if (state->mode == ALARM_MODE)
    {
        if (state->innerMode == DEFAULT)
        {
            gotoxy(MODE_LINE_X - 5, MODE_LINE_Y);
            printf("A : 알람 설정  B : 알람 ON/OFF  C : 현재 시간");
        }
        else if (state->innerMode == ALARM_SET)
        {
            gotoxy(MODE_LINE_X - 7, MODE_LINE_Y);
            printf("A : 알람 보기  B : 시간 증가  C : 설정 대상 변경");
        }
    }
}
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
    state->timeKeepingUnit = TIME_KEEPING_SEC;
    state->alarmUnit = ALARM_HOUR;

    //state's time struct initialize
    state->currentTime.year = 2019;
    state->currentTime.month = 1;
    state->currentTime.day = 1;
    state->currentTime.hour = 0;
    state->currentTime.min = 59;
    state->currentTime.sec = 59;
    state->currentTime.dayOfWeek = 0;
    state->stopWatch.min = 0;
    state->stopWatch.sec = 0;
    state->stopWatch.msec = 0;

    state->lapTime.min = 0;
    state->lapTime.sec = 0;
    state->lapTime.msec = 0;
    alarm->alarmIndicator = ON;
    alarm->alarmState = OFF;
    alarm->hour = 1;
    alarm->min = 0;
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
}
void mainProcessB(StateData *state, AlarmData *alarm, char button) // main process for button 'B'
{
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
}
void mainProcessC(StateData *state, AlarmData *alarm, char button) // main process for button 'C'
{
    if (state->innerMode == DEFAULT) //mode change
        state->mode = (state->mode + 1) % 3;
    else if (state->mode == TIME_KEEPING_MODE)
        state->timeKeepingUnit = (state->timeKeepingUnit + 1) % 6;

    else if (state->mode == ALARM_MODE)
        state->alarmUnit = (state->alarmUnit + 1) % 2;
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
    state->timeKeepingUnit = TIME_KEEPING_SEC;
    modeChangePrint();
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
    state->stopWatch.min = 0;
    state->stopWatch.sec = 0;
    state->stopWatch.msec = 0;
    modeChangePrint();
}

void stopWatchRun(StateData *state)
{
    state->innerMode = STOP_WATCH_RUN;
}
void stopWatchLaptime(StateData *state)
{
    if (state->mode == STOP_WATCH_MODE)
        state->innerMode = STOP_WATCH_LAPTIME;
    // state->lapTime.year = 0;
    state->lapTime.min = state->stopWatch.min;
    state->lapTime.sec = state->stopWatch.sec;
    state->lapTime.msec = state->stopWatch.msec;
    // lapTime->year = 0;
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
    modeChangePrint();
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
            state->currentTime.dayOfWeek = (state->currentTime.dayOfWeek + 365) % 7;
            break;
        case TIME_KEEPING_MONTH:
            state->currentTime.month = (state->currentTime.month) % 12 + 1;
            state->currentTime.dayOfWeek = (state->currentTime.dayOfWeek + 30) % 7;
            if (state->currentTime.month == 1)
                state->currentTime.dayOfWeek = (state->currentTime.dayOfWeek + 4) % 7;
            break;
        case TIME_KEEPING_DAY:
            state->currentTime.day = (state->currentTime.day) % dayOfMonth + 1;
            state->currentTime.dayOfWeek = (state->currentTime.dayOfWeek + 1) % 7;
            if (state->currentTime.day == 1)
                state->currentTime.dayOfWeek = (state->currentTime.dayOfWeek + 5) % 7;
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

void modeChangePrint()
{
    gotoxy(DATE_LINE_X - 10, DATE_LINE_Y);
    printf("                                        ");
    gotoxy(TIME_LINE_X - 10, TIME_LINE_Y);
    printf("                                        ");
    gotoxy(MODE_LINE_X - 10, MODE_LINE_Y);
    printf("                                                                                        ");
    gotoxy(MODE_LINE_X - 10, MODE_LINE_Y + 2);
    printf("                                                                                        ");
}