#ifndef GREENHOUSE_H
#define GREENHOUSE_H

#include <stdint.h>
#include <string.h>
#include "datetime.h"

// Enum for screen types
typedef enum
{
    MAIN_SCREEN,
    DATE_TIME_CONFIGURATION,
    NONE
    // Add more screens here as needed
} ScreenType;

typedef enum
{
    FALSE,
    TRUE
} Boolean;

// Structure to hold global greenhouse state
typedef struct
{
    // Keypad and input state
    unsigned int keypadValue;
    unsigned int lastKeypadValue;

    // Display state
    ScreenType currentScreen;
    ScreenType lastScreenDisplay; // Track last displayed screen to avoid re-rendering

    // Sensor values
    float photoValue;
    float temperature;

    // Calendar/Date/Time variables
    DateTime currentDateTime;
    Boolean inTimestampConfiguration;
} GreenHouse;

// Global greenhouse instance
extern GreenHouse greenhouse;

// Display functions
void initGreenHouse(void);
void displayScreen(void);
void displayDateTime(void);

#endif // GREENHOUSE_H
