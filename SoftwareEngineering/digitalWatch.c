#include "digitalWatch.h"
int main(void)
{
    char inputButton[INPUT_BUTTON_SIZE] = {0};         //User's button input. It will be processed and stored at processed button.
    char processedButton[PROCESSED_BUTTON_SIZE] = {0}; //processed 4(random A~D -> D-C-B-A) button from inputButton.
    char currentButton;
    StateData state;
    AlarmData alarm;
    PassingData pass;
    ButtonData button;
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
    int alarmThreadCounter = 0;
    int alarmState = OFF;
    setDefault(&state, &alarm); //state, alarm initialize
    pass.alarm = &alarm;
    alarm.alarmState = &alarmState;
    pass.backlightThreadBoundaryLine = &backlightThreadBoundaryLine;
    pass.alarmThreadBoundaryLine = &alarmThreadBoundaryLine;
    pass.mainBoundaryLine = &mainBoundaryLine;
    pass.backlightThreadCounter = &backlightThreadCounter;
    pass.alarmThreadCounter = &alarmThreadCounter;

    button.inputButton = inputButton;
    button.processedButton = processedButton;
    button.buttonLength = &buttonLength;
    button.buttonThreadBoundaryLine = &buttonThreadBoundaryLine;
    button.mainBoundaryLine = &mainBoundaryLine;
    mainBoundaryLine = UNLOCK;
    buttonThreadBoundaryLine = LOCK;
    pthread_create(&buttonThread, NULL, &buttonThreadFunction, (void *)&button);
    setDefault(&state, &alarm);
    system("clear");
    while (1)
    {
        if (isAlarmTime(&state, &alarm) && alarmThreadCounter == 0)
        {
            pthread_create(&alarmThread, NULL, &alarmThreadFunction, (void *)&pass);
            pthread_detach(alarmThread);
        }

        if (*(button.buttonThreadBoundaryLine) == UNLOCK)
        {
            for (i = 0; i < buttonLength; i++)
            {
                currentButton = processedButton[i];
                pass.button = &currentButton;
                if (currentButton == 'D')
                {
                    pthread_create(&backlightThread, NULL, &backlightThreadFunction, (void *)&pass);
                    pthread_detach(backlightThread);
                }
                decideMainProcess(&state, &alarm, currentButton);
            }
            mainBoundaryLine = CLEAR;
            *(button.buttonThreadBoundaryLine) = LOCK;
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
        gotoxy(BUTTON_INPUT_X, BUTTON_INPUT_Y);
        printf("Button : ");
        gotoxy(BUTTON_INPUT_X + 9, BUTTON_INPUT_Y);
    }
    pthread_join(alarmThread, NULL);
    pthread_join(buttonThread, NULL);
    return 0;
}

void *alarmThreadFunction(void *_pass) //Alarm controller thread. need to pass tm data
{
    //time = 1000 -> 1
    //time = 100 -> 10
    //time = 10 -> 100
    PassingData *pass = (PassingData *)_pass;
    int i;
    *(pass->alarmThreadCounter) = *(pass->alarmThreadCounter) + 1;
    for (i = 0; i < (5000 / ALARM_SLEEP); i++)
    {
        if (i % (1000 / ALARM_SLEEP / 2) == 0)
            printf("\a\n");
        Sleep(ALARM_SLEEP);
        if (*(pass->alarm->alarmState) == OFF)
        {
            pass->alarm->alarmIndicator = OFF;
            *(pass->alarmThreadCounter) = *(pass->alarmThreadCounter) - 1;
            break;
        }
    }
    *(pass->alarmThreadCounter) = *(pass->alarmThreadCounter) - 1;
    if (*(pass->backlightThreadCounter) == 0)
        *(pass->alarm->alarmState) = OFF;
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

int isAlarmTime(StateData *state, AlarmData *alarm)
{
    return (alarm->alarmIndicator && alarm->hour == state->currentTime.hour && alarm->min == state->currentTime.min && state->currentTime.sec == 0);
}