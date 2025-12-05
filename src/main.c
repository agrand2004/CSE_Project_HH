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

    // Clear the screen
    deactivateKeyPadAndActivateLCD();
    lcdClearText();
    lcdGotoXY(0, 0);

    initGreenHouse();

    // Main loop
    while (1)
    {
        // Display the appropriate screen
        displayScreen();

        if (tempDataReady)
        {
            // Read temperature
            greenhouse.temperature = getTemperature();
            greenhouse.needDisplayUpdate = TRUE;
        }

        // Read keypad
        activateKeyPadAndDeactivateLCD();
        readKeypad(&greenhouse.keypadValue);

        handleKeypadInput();
        delay(50); // Debouncing delay
    }

    return 0;
}
