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
    int alarmState;
    alarm.alarmState = &alarmState;
    pass.alarm = &alarm;
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
    setDefault(&state, &alarm);
    pthread_create(&buttonThread, NULL, &buttonThreadFunction, (void *)&button);
    system("clear");
    while (1)
    {
        if (isAlarmTime(&state, &alarm) && alarmThreadCounter == 0 && state.innerMode != TIME_KEEPING_SET && state.innerMode != ALARM_SET)
        {
            alarmState = ON;
            pthread_create(&alarmThread, NULL, &alarmThreadFunction, (void *)&pass);
            pthread_detach(alarmThread);
        }

        if (*(button.buttonThreadBoundaryLine) == UNLOCK)
        {
            for (i = 0; i < buttonLength; i++)
            {
                currentButton = processedButton[i];
                pass.button = &currentButton;
                if (alarmState == ON)
                {
                    alarmState = OFF;
                    continue;
                }
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
