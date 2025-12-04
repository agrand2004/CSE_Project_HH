#include "greenhouse.h"
#include "lcd_function.h"
#include "utils.h"

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
}

// Display screen based on current screen type
// This function checks if the screen has changed and only re-renders if needed
void displayScreen(void)
{
    // Only re-render if the screen has changed
    if (greenhouse.currentScreen == greenhouse.lastScreenDisplay)
    {
        return;
    }

    // Update the last displayed screen
    greenhouse.lastScreenDisplay = greenhouse.currentScreen;

    // Clear the screen for fresh display
    lcdClearText();
    lcdGotoXY(0, 0);

    // Display based on current screen type
    switch (greenhouse.currentScreen)
    {
    case MAIN_SCREEN:
        // Display main screen content
        // Temperature on line 0, Light on line 1, etc.
        displayDateTime();
        lcdPrintAt(0, 1, "Light: ");
        lcdPrintAt(0, 2, "Temperature: ");
        break;
    case DATE_TIME_CONFIGURATION:
        // Display date configuration screen
        lcdPrintAt(0, 0, "Set Date:");
        lcdGotoXY(0, 1);
        lcdPuts("DD/MM/YYYY");
        break;
    default:
        lcdPrintAt(0, 0, "Unknown Screen");
        break;
    }
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
