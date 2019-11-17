#include "digitalWatch.h"
int main(void)
{
    char *weekOfDays[7] =
        {
            "TU",
            "TH",
            "WE",
            "FR",
            "SA",
            "SU",
            "MO"};
    char inputButton[INPUT_BUTTON_SIZE] = {0};         //User's button input. It will be processed and stored at processed button.
    char processedButton[PROCESSED_BUTTON_SIZE] = {0}; //processed 4(random A~D -> D-C-B-A) button from inputButton.
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
    int backlightThreadCounter = 0;
    setDefault(&state, &alarm); //state, alarm initialize
    currentTime = localtime(&timer);
    pass.alarm = &alarm;
    pass.backlightThreadBoundaryLine = &backlightThreadBoundaryLine;
    pass.alarmThreadBoundaryLine = &alarmThreadBoundaryLine;
    pass.mainBoundaryLine = &mainBoundaryLine;
    pass.backlightThreadCounter = &backlightThreadCounter;

    button.inputButton = inputButton;
    button.processedButton = processedButton;
    button.buttonLength = &buttonLength;
    button.buttonThreadBoundaryLine = &buttonThreadBoundaryLine;
    button.mainBoundaryLine = &mainBoundaryLine;
    mainBoundaryLine = UNLOCK;
    buttonThreadBoundaryLine = LOCK;
    pthread_create(&alarmThread, NULL, &alarmThreadFunction, (void *)&pass);
    pthread_create(&buttonThread, NULL, &buttonThreadFunction, (void *)&button);
    setDefault(&state, &alarm);
    system("clear");
    while (1)
    {
        // system("clear");
        // gotoxy(50, 50);
        if (*(button.buttonThreadBoundaryLine) == UNLOCK)
        {
            for (i = 0; i < buttonLength; i++)
            {
                currentButton = processedButton[i];
                pass.button = &currentButton;
                /**/
                if (currentButton == 'D')
                {
                    pthread_create(&backlightThread, NULL, &backlightThreadFunction, (void *)&pass);
                    pthread_detach(backlightThread);
                }
                /**/
                decideMainProcess(&state, &alarm, currentButton);
            }
            mainBoundaryLine = CLEAR;
            *(button.buttonThreadBoundaryLine) = LOCK;
            // gotoxy(BUTTON_INPUT_X, BUTTON_INPUT_Y);
        }
        if (getButtonLength(processedButton) > 0)
        {
            gotoxy(BUTTON_INPUT_X + 9, BUTTON_INPUT_Y);
            printf("      ");
        }
        if (*(button.buttonThreadBoundaryLine) == CLEAR)
            mainBoundaryLine = UNLOCK;
        timeProcess(&state, &alarm);
        showWatch(&state, &alarm);
        // gotoxy(BUTTON_INPUT_X + 9, BUTTON_INPUT_Y);
        // printf("            ");
        gotoxy(BUTTON_INPUT_X, BUTTON_INPUT_Y);
        printf("Button : ");
        gotoxy(BUTTON_INPUT_X + 9, BUTTON_INPUT_Y);
    }
    pthread_join(alarmThread, NULL);
    pthread_join(buttonThread, NULL);
    return 0;
}

void timeProcess(StateData *state, AlarmData *alarm)
{
    TimeSet *currentTime = &state->currentTime;
    static int timeKeepAddCheck = 0;
    static int stopWatchAddCheck = 0;
    timeKeepAddCheck++;
    int dayOfMonth = 30 + ((currentTime->month <= 7 && currentTime->month % 2) || //31 or 30
                           (currentTime->month >= 8 && !(currentTime->month % 2)));
    Sleep(TIME_KEEPING_SLEEP);
    if (timeKeepAddCheck % (1000 / TIME_KEEPING_SLEEP) == 0)
        currentTime->sec++;
    if (currentTime->sec >= 60)
    {
        currentTime->sec = 0;
        currentTime->min++;
    }
    if (currentTime->min >= 60)
    {
        currentTime->min = 0;
        currentTime->hour++;
    }
    if (currentTime->hour >= 24)
    {
        currentTime->hour = 0;
        currentTime->day++;
        currentTime->dayOfWeek = currentTime->dayOfWeek % 7;
    }
    if (currentTime->day >= dayOfMonth + 1)
    {
        currentTime->day = 1;
        currentTime->month++;
    }
    if (currentTime->month >= 13)
    {
        currentTime->month = 1;
        currentTime->year++;
    }
    if (state->mode == STOP_WATCH_MODE && (state->innerMode == STOP_WATCH_RUN || state->innerMode == STOP_WATCH_LAPTIME))
    {
        TimeSet *stopWatchTime = &state->stopWatch;
        Sleep(STOP_WATCH_SLEEP);
        if (stopWatchAddCheck % (1000 / (STOP_WATCH_SLEEP * 10)) == 0)
            stopWatchTime->msec++;
        if (stopWatchTime->msec >= 100)
        {
            stopWatchTime->msec = 0;
            stopWatchTime->sec++;
        }
        if (stopWatchTime->sec >= 60)
        {
            stopWatchTime->sec = 0;
            stopWatchTime->min++;
        }
    }
}
void showWatch(StateData *state, AlarmData *alarm) // show watch based on StateData
{
    if (state->mode == TIME_KEEPING_MODE) // Time Keeping Mode
    {
        TimeSet *currentTime = &state->currentTime;
        modeChangePrint();
        gotoxy(DATE_LINE_X, DATE_LINE_Y);
        printf("%04d년 %02d월 %02d일", currentTime->year, currentTime->month, currentTime->day);
        gotoxy(TIME_LINE_X, TIME_LINE_Y);
        printf("%02d:%02d:%02d", currentTime->hour, currentTime->min, currentTime->sec);
    }
    else if (state->mode == STOP_WATCH_MODE) // Stop Watch Mode
    {
        TimeSet *stopWatchTime = &state->stopWatch;
        modeChangePrint();
        gotoxy(TIME_LINE_X, TIME_LINE_Y);
        printf("%02d:%02d:%02d", stopWatchTime->min, stopWatchTime->sec, stopWatchTime->msec);
    }
    else if (state->mode == ALARM_MODE) // Alarm Mode
    {
    }
}

void *alarmThreadFunction(void *_pass) //Alarm controller thread. need to pass tm data
{
    PassingData *pass = (PassingData *)_pass;
}

void *buttonThreadFunction(void *_buttonList)
{
    ButtonData *buttonList = _buttonList;
    int buttonProcessed = 0;
    while (1)
    {
        if (*(buttonList->mainBoundaryLine) == UNLOCK && buttonProcessed == 0)
        {
            // gotoxy(BUTTON_INPUT_X + 9, BUTTON_INPUT_Y);
            // printf("            ");
            // gotoxy(BUTTON_INPUT_X, BUTTON_INPUT_Y);
            // printf("Button : ");
            scanf("%s", buttonList->inputButton);
            buttonProcess(buttonList->inputButton, buttonList->processedButton);
            *(buttonList->buttonLength) = getButtonLength(buttonList->processedButton);
            *(buttonList->buttonThreadBoundaryLine) = UNLOCK;
            *(buttonList->mainBoundaryLine) == LOCK;
            buttonProcessed = 1;
        }
        else if (*(buttonList->mainBoundaryLine) == CLEAR && *(buttonList->buttonThreadBoundaryLine) != CLEAR)
        {
            buttonInitialize(buttonList->processedButton, 0, PROCESSED_BUTTON_SIZE);

            *(buttonList->mainBoundaryLine) = LOCK;
            *(buttonList->buttonThreadBoundaryLine) = CLEAR;
            buttonProcessed = 0;
        }
    }
}