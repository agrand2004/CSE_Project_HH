#include "system_sam3x.h"
#include "at91sam3x8.h"
#include "core_cm3.h"
#include "pins.h"
#include "lcd_function.h"
#include "keypad_function.h"
#include "temp_sensor_function.h"
#include "photoresistor_function.h"
#include "servomotor_function.h"

#define DELAY_MS 1000 // 1000ms delay for reading temperature

unsigned int keypadValue = 0;
unsigned int lastKeypadValue = 0;
unsigned int tickCounter = 0;
float photoValue = 0;

void initSysTick(uint32_t ticks)
{
    // Disable SysTick during setup
    SysTick->CTRL = 0;

    // Set reload register (subtract 1 because counter counts from LOAD to 0)
    SysTick->LOAD = ticks - 1;

    // Clear current value register
    SysTick->VAL = 0;

    // Enable SysTick with processor clock and interrupt
    // CTRL bits: [0]=ENABLE, [1]=TICKINT (interrupt enable), [2]=CLKSOURCE (processor clock)
    SysTick->CTRL = SysTick_CTRL_ENABLE_Msk |
                    SysTick_CTRL_TICKINT_Msk |
                    SysTick_CTRL_CLKSOURCE_Msk;
}

// Convert float to string (simple version without sprintf)
void floatToString(float value, char *buffer)
{
    int intPart = (int)value;
    int decPart = (int)((value - intPart) * 10);
    if (decPart < 0)
        decPart = -decPart;

    // Convert integer part
    int i = 0;
    if (intPart < 0)
    {
        buffer[i++] = '-';
        intPart = -intPart;
    }

    if (intPart == 0)
    {
        buffer[i++] = '0';
    }
    else
    {
        char temp[10];
        int j = 0;
        while (intPart > 0)
        {
            temp[j++] = '0' + (intPart % 10);
            intPart /= 10;
        }
        while (j > 0)
        {
            buffer[i++] = temp[--j];
        }
    }

    // Add decimal part
    buffer[i++] = '.';
    buffer[i++] = '0' + decPart;
    buffer[i] = '\0';
}

// ===== Delay function =====
void delay(unsigned int us)
{
    for (int i = 0; i < us; i++)
        asm("nop");
}

void activateKeyPadAndDeactivateLCD(void)
{
    // deactivate the 74HC245 bus of the LCD
    AT91C_BASE_PIOC->PIO_SODR = (1 << PIN_74HC245_LCD_G); // LCD 74HC245 OFF (G=1)
    // activate the 74HC245 of the keypad
    AT91C_BASE_PIOD->PIO_CODR = (1 << PIN_74HC245_KB_G);
}

void deactivateKeyPadAndActivateLCD(void)
{
    // deactivate the 74HC245 of the keypad
    AT91C_BASE_PIOD->PIO_SODR = (1 << PIN_74HC245_KB_G); // Keypad 74HC245 OFF (G=1)
    // activate the 74HC245 of the LCD
    AT91C_BASE_PIOC->PIO_CODR = (1 << PIN_74HC245_LCD_G);
}

void SysTick_Handler(void)
{
    tickCounter++;

    if (tickCounter >= DELAY_MS / 100)
    {
        tickCounter = 0;
        readTempSensor();
        photoValue = readPhotoresistor();
    }
}

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
