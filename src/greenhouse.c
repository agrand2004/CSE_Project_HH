#include "greenhouse.h"

// Global greenhouse instance
GreenHouse greenhouse;

void initGreenHouse(void)
{
    greenhouse.keypadValue = 0;
    greenhouse.lastKeypadValue = 0;
    greenhouse.currentScreen = MAIN_SCREEN;
    greenhouse.lastScreenDisplay = NONE;
    greenhouse.photoValue = 0.0f;
    greenhouse.temperature = 0.0;
    greenhouse.currentDateTime.seconds = 0;
    greenhouse.currentDateTime.minutes = 0;
    greenhouse.currentDateTime.hours = 0;
    greenhouse.currentDateTime.day = 0;
    greenhouse.currentDateTime.month = 0;
    greenhouse.currentDateTime.year = 0;
    greenhouse.inTimestampConfiguration = FALSE;
    greenhouse.needDisplayUpdate = TRUE;
}

void displayMainScreen(void)
{
    // Variables for temperature and photoresistor reading
    char tempBuffer[20];
    char photoBuffer[20];

    if (tempDataReady)
    {
        // Read temperature
        float temperature = getTemperature();
        greenhouse.temperature = temperature;

        // Display temperature on LCD
        lcdPrintAt(0, 2, "Temperature: ");
        lcdGotoXY(13, 2); // Second line
        floatToString(temperature, tempBuffer);
        lcdPuts(tempBuffer);
        lcdPuts("C");
        lcdFinishLine(2, 13 + strlen(tempBuffer) + 1); // Clear rest of line
    }

    // Display photoresistor value on LCD
    // TODO: remove this when no longer needed
    lcdPrintAt(0, 1, "Light: ");
    lcdGotoXY(7, 1); // Return to first line
    floatToString(greenhouse.photoValue, photoBuffer);
    lcdPuts(photoBuffer);
    lcdPuts("V");
    lcdFinishLine(1, 7 + strlen(photoBuffer) + 1); // Clear rest of line

    // Display the date and time
    displayDateTime();
}

void displayDateTimeConfigurationScreen(void)
{
    lcdPrintAt(0, 0, "Set Date:");
    lcdGotoXY(0, 1);
    lcdPuts("DD/MM/YYYY");
}

// Display screen based on current screen type
// This function checks if the screen has changed and only re-renders if needed
void displayScreen(void)
{
    // Only re-render if the screen has changed or if an update is needed
    if (!greenhouse.needDisplayUpdate && greenhouse.currentScreen == greenhouse.lastScreenDisplay)
    {
        return;
    }

    if (greenhouse.currentScreen != greenhouse.lastScreenDisplay)
    {
        lcdClearText();
        lcdGotoXY(0, 0);
        // Update the last displayed screen
        greenhouse.lastScreenDisplay = greenhouse.currentScreen;
    }
    if (greenhouse.needDisplayUpdate)
    {
        greenhouse.needDisplayUpdate = FALSE;
    }

    // Ensure LCD is active
    deactivateKeyPadAndActivateLCD();

    // Display based on current screen type
    switch (greenhouse.currentScreen)
    {
    case MAIN_SCREEN:
        // Display main screen content
        displayMainScreen();
        break;
    case DATE_TIME_CONFIGURATION_SCREEN:
        // Display date/time configuration screen content
        displayDateTimeConfigurationScreen();
        break;
    default:
        lcdPrintAt(0, 0, "Unknown Screen");
        break;
    }

    // Reactivate keypad after updating display
    activateKeyPadAndDeactivateLCD();
}

void displayDateTime(void)
{
    char timeBuffer[20];
    char dateBuffer[20];

    timeToString(greenhouse.currentDateTime, timeBuffer, sizeof(timeBuffer));
    dateToString(greenhouse.currentDateTime, dateBuffer, sizeof(dateBuffer));

    // Display on LCD
    lcdPrintAt(0, 0, dateBuffer);  // Date on line 0
    lcdPrintAt(22, 0, timeBuffer); // Time on line 0 (half way)
}
