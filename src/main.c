#include "system_sam3x.h"
#include "at91sam3x8.h"
#include "core_cm3.h"
#include "pins.h"
#include "lcd_function.h"
#include "keypad_function.h"
#include "temp_sensor_function.h"
#include "photoresistor_function.h"
#include "servomotor_function.h"
#include "systick.h"
#include "utils.h"
#include "greenhouse.h"
#include <string.h>

int main(void)
{
    // Initialize system
    SystemInit();
    initSysTick(8400000); // 0.1s tick at 84MHz

    initKeypad();
    lcdInit();
    tempSensorInit();
    photoresistorInit();
    servomotorInit();
    deactivateKeyPadAndActivateLCD();

    // Clear the screen
    lcdClearText();
    lcdGotoXY(0, 0);

    initGreenHouse();

    // Main loop
    while (1)
    {
        // Display the appropriate screen
        displayScreen();

        // if (tempDataReady && greenhouse.currentScreen == MAIN_SCREEN)
        // {
        //     // Read temperature
        //     float temperature = getTemperature();
        //     greenhouse.temperature = temperature;

        //     // Display photoresistor value on LCD
        //     lcdGotoXY(7, 1); // Return to first line
        //     floatToString(greenhouse.photoValue, photoBuffer);
        //     lcdPuts(photoBuffer);
        //     lcdPuts("V");
        //     lcdFinishLine(1, 7 + strlen(photoBuffer) + 1); // Clear rest of line

        //     // Display temperature on LCD
        //     deactivateKeyPadAndActivateLCD();
        //     lcdGotoXY(13, 2); // Second line
        //     floatToString(temperature, tempBuffer);
        //     lcdPuts(tempBuffer);
        //     lcdPuts("C");
        //     lcdFinishLine(2, 13 + strlen(tempBuffer) + 1); // Clear rest of line

        //     displayDateTime();
        // }

        // Read keypad
        activateKeyPadAndDeactivateLCD();
        readKeypad(&greenhouse.keypadValue);

        if (greenhouse.keypadValue != 0 && greenhouse.keypadValue != greenhouse.lastKeypadValue)
        {
            if (greenhouse.keypadValue == 12)
            {
                // Switch to date/time configuration screen
                greenhouse.currentScreen = DATE_TIME_CONFIGURATION_SCREEN;
            }
            else if (greenhouse.keypadValue == 10)
            {
                // Switch back to main screen
                greenhouse.currentScreen = MAIN_SCREEN;
            }
            servomotorWriteValue((greenhouse.keypadValue - 1) * 10); // Map keypad value to servo angle (0-180 degrees)
        }
        else if (greenhouse.keypadValue == 0)
        {
            greenhouse.lastKeypadValue = 0;
        }

        delay(50); // Debouncing delay
    }

    return 0;
}
