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

unsigned int keypadValue = 0;
unsigned int lastKeypadValue = 0;
float photoValue = 0;

int main(void)
{
    // Initialize system
    SystemInit();
    initSysTick(4200000); // 0.1s tick at 42MHz

    initKeypad();
    lcdInit();
    tempSensorInit();
    photoresistorInit();
    servomotorInit();
    deactivateKeyPadAndActivateLCD();

    // Clear the screen
    lcdClearText();
    lcdGotoXY(0, 0);

    // Variables for temperature and photoresistor reading
    char tempBuffer[20];
    char photoBuffer[20];

    // Main loop
    while (1)
    {
        if (tempDataReady)
        {
            // Read temperature
            float temperature = getTemperature();

            // Display temperature on LCD
            deactivateKeyPadAndActivateLCD();
            lcdGotoXY(0, 1); // Second line
            lcdPuts("Temp: ");
            floatToString(temperature, tempBuffer);
            lcdPuts(tempBuffer);
            lcdPuts("C");

            // Display photoresistor value on LCD
            lcdGotoXY(0, 0); // Return to first line
            lcdPuts("Light: ");
            floatToString(photoValue, photoBuffer);
            lcdPuts(photoBuffer);
            lcdPuts("V");
        }

        // Read keypad
        activateKeyPadAndDeactivateLCD();
        readKeypad(&keypadValue);

        if (keypadValue != 0 && keypadValue != lastKeypadValue)
        {
            servomotorWriteValue((keypadValue - 1) * 10); // Map keypad value to servo angle (0-180 degrees)
        }
        else if (keypadValue == 0)
        {
            lastKeypadValue = 0;
        }

        delay(50); // Debouncing delay
    }

    return 0;
}
