#include "greenhouse.h"
#include "datetime.h"

uint8_t monthDayCnt[] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
uint8_t monthDayCntLeap[] = {31, 29, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};

// Helper function to check if a year is a leap year
static Boolean isLeapYear(uint16_t year)
{
    if (year % 400 == 0)
        return TRUE;
    if (year % 100 == 0)
        return FALSE;
    if (year % 4 == 0)
        return TRUE;
    return FALSE;
}

// Helper function to get the number of days in a given month
static uint8_t getDaysInMonth(uint8_t month, uint16_t year)
{
    if (month <= 0 || month > 12)
        return 0;

    if (isLeapYear(year))
        return monthDayCntLeap[month - 1];
    else
        return monthDayCnt[month - 1];
}

// Add one second to the current date/time
void addOneSecond(void)
{
    // Increment seconds
    greenhouse.currentDateTime.seconds++;

    // Check if we need to carry over to minutes
    if (greenhouse.currentDateTime.seconds >= 60)
    {
        greenhouse.currentDateTime.seconds = 0;
        greenhouse.currentDateTime.minutes++;

        // Check if we need to carry over to hours
        if (greenhouse.currentDateTime.minutes >= 60)
        {
            greenhouse.currentDateTime.minutes = 0;
            greenhouse.currentDateTime.hours++;

            // Check if we need to carry over to days
            if (greenhouse.currentDateTime.hours >= 24)
            {
                greenhouse.currentDateTime.hours = 0;
                greenhouse.currentDateTime.day++;

                // Check if we need to carry over to months
                uint8_t daysInMonth = getDaysInMonth(greenhouse.currentDateTime.month, greenhouse.currentDateTime.year);
                if (greenhouse.currentDateTime.day > daysInMonth)
                {
                    greenhouse.currentDateTime.day = 1;
                    greenhouse.currentDateTime.month++;

                    // Check if we need to carry over to years
                    if (greenhouse.currentDateTime.month > 12)
                    {
                        greenhouse.currentDateTime.month = 1;
                        greenhouse.currentDateTime.year++;
                    }
                }
            }
        }
    }
}

// Function to update the date and time of the greenhouse
// Called by SysTick every second
void updateDateTime(void)
{
    if (greenhouse.currentScreen == DATE_TIME_CONFIGURATION_SCREEN)
    {
        return; // Do not update if in configuration mode
    }

    addOneSecond();
}

// Validate if a DateTime structure is valid
Boolean isDateTimeValid(DateTime dt)
{
    // Validate seconds, minutes, hours
    if (dt.seconds > 59 || dt.minutes > 59 || dt.hours > 23)
        return FALSE;

    // Validate month
    if (dt.month < 1 || dt.month > 12)
        return FALSE;

    // Validate day based on month
    uint8_t maxDays = getDaysInMonth(dt.month, dt.year);
    if (dt.day < 1 || dt.day > maxDays)
        return FALSE;

    return TRUE;
}

// Configure date and time interactively via keypad
// Input format: DD/MM/YYYY hh:mm:ss
// Positions: 0=D1, 1=D2, 2=M1, 3=M2, 4=Y1, 5=Y2, 6=Y3, 7=Y4, 8=h1, 9=h2, 10=m1, 11=m2, 12=s1, 13=s2
void configureDateTime(void)
{
    char inputBuffer[20];
    uint8_t position = 0;
    DateTime tempDateTime;

    // Initialize display buffer
    for (int i = 0; i < 20; i++)
        inputBuffer[i] = '_';

    inputBuffer[2] = '/';
    inputBuffer[5] = '/';
    inputBuffer[10] = ' ';
    inputBuffer[13] = ':';
    inputBuffer[16] = ':';
    inputBuffer[19] = '\0';

    // Position mapping for display (skip separators)
    uint8_t positionMap[] = {0, 1, 3, 4, 6, 7, 8, 9, 11, 12, 14, 15, 17, 18};

    deactivateKeyPadAndActivateLCD();

    // Write instructions
    lcdClearText();
    lcdPrintAt(0, 0, "Set Date/Time:");
    lcdPrintAt(0, LCD_ROWS - 1, "*: Cancel");

    greenhouse.needDisplayUpdate = TRUE;

    while (position < 14) // 14 digits to enter: DD MM YYYY hh mm ss
    {
        // Display current input
        if (greenhouse.needDisplayUpdate)
        {
            lcdGotoXY(0, 1);
            lcdPuts(inputBuffer);
        }

        // Wait for keypad input
        activateKeyPadAndDeactivateLCD();
        while (greenhouse.keypadValue == greenhouse.lastKeypadValue || greenhouse.keypadValue == 0)
        {
            if (greenhouse.keypadValue == 0)
            {
                greenhouse.lastKeypadValue = 0;
            }
            readKeypad(&greenhouse.keypadValue);
            delay(50); // Debounce delay
        }

        greenhouse.lastKeypadValue = greenhouse.keypadValue;
        unsigned int key = greenhouse.keypadValue;

        deactivateKeyPadAndActivateLCD();

        // Handle input
        if (key >= 1 && key <= 9) // Number keys
        {
            inputBuffer[positionMap[position]] = '0' + key;
            greenhouse.needDisplayUpdate = TRUE;
            position++;
        }
        if (key == 11) // 0 key
        {
            inputBuffer[positionMap[position]] = '0';
            greenhouse.needDisplayUpdate = TRUE;
            position++;
        }
        else if (key == 10) // * key - cancel and return to main screen
        {
            greenhouse.currentScreen = MAIN_SCREEN;
            return;
        }
        if (position == 14) // All digits entered
        {
            // Parse the input
            tempDateTime.day = (inputBuffer[0] - '0') * 10 + (inputBuffer[1] - '0');
            tempDateTime.month = (inputBuffer[3] - '0') * 10 + (inputBuffer[4] - '0');
            tempDateTime.year = (inputBuffer[6] - '0') * 1000 + (inputBuffer[7] - '0') * 100 +
                                (inputBuffer[8] - '0') * 10 + (inputBuffer[9] - '0');
            tempDateTime.hours = (inputBuffer[11] - '0') * 10 + (inputBuffer[12] - '0');
            tempDateTime.minutes = (inputBuffer[14] - '0') * 10 + (inputBuffer[15] - '0');
            tempDateTime.seconds = (inputBuffer[17] - '0') * 10 + (inputBuffer[18] - '0');

            // Validate the entered date/time
            if (isDateTimeValid(tempDateTime))
            {
                // Valid! Save it
                greenhouse.currentDateTime = tempDateTime;
                lcdPrintAt(0, 1, inputBuffer); // Show final input
                lcdPrintAt(0, 3, "New date set!");
                lcdPrintAt(0, 4, "Press any key to exit");
                activateKeyPadAndDeactivateLCD();
                while (greenhouse.keypadValue == greenhouse.lastKeypadValue || greenhouse.keypadValue == 0)
                {
                    if (greenhouse.keypadValue == 0)
                    {
                        greenhouse.lastKeypadValue = 0;
                    }
                    readKeypad(&greenhouse.keypadValue);
                    delay(50); // Debounce delay
                }
                greenhouse.lastKeypadValue = greenhouse.keypadValue;
                deactivateKeyPadAndActivateLCD();
                greenhouse.currentScreen = MAIN_SCREEN;
                return;
            }
            else
            {
                // Invalid! Reset and try again
                position = 0;
                for (int i = 0; i < 20; i++)
                    inputBuffer[i] = '_';
                inputBuffer[2] = '/';
                inputBuffer[5] = '/';
                inputBuffer[10] = ' ';
                inputBuffer[13] = ':';
                inputBuffer[16] = ':';
                inputBuffer[19] = '\0';

                // Display error message
                lcdPrintAt(0, 3, "Invalid Date!");
                lcdPrintAt(0, 4, "Try again...");
            }
        }
    }
}