#include "system_sam3x.h"
#include "at91sam3x8.h"
#include "core_cm3.h"
#include "pins.h"
#include "keypad_function.h"

// Keypad mapping
const char keypadMap[12] = {
    '1', // 1
    '2', // 2
    '3', // 3
    '4', // 4
    '5', // 5
    '6', // 6
    '7', // 7
    '8', // 8
    '9', // 9
    '*', // 10
    '0', // 11
    '#'  // 12
};

// Initialize keypad
void initKeypad(void)
{
    // Enable peripheral clock for PIOC and PIOD
    AT91C_BASE_PMC->PMC_PCER = (1 << AT91C_ID_PIOC) | (1 << AT91C_ID_PIOD);

    // Configure 74HC245 control pin
    // Enable the Octal bus GPIO line
    AT91C_BASE_PIOD->PIO_PER = (1 << PIN_74HC245_KB_G);  // Enable PIO control PD2
    AT91C_BASE_PIOD->PIO_OER = (1 << PIN_74HC245_KB_G);  // Enable output for PD2
    AT91C_BASE_PIOD->PIO_CODR = (1 << PIN_74HC245_KB_G); // Set LOW to enable transceiver

    // Enable PIO control for ALL keypad pins at once
    AT91C_BASE_PIOC->PIO_PER = (1 << PIN_KB_1) | (1 << PIN_KB_3) | (1 << PIN_KB_5) |                  // Configure outputs (columns)
                               (1 << PIN_KB_2) | (1 << PIN_KB_4) | (1 << PIN_KB_6) | (1 << PIN_KB_7); // Configure inputs (rows)

    // Configure outputs (columns)
    AT91C_BASE_PIOC->PIO_OER = (1 << PIN_KB_1) | (1 << PIN_KB_3) | (1 << PIN_KB_5);  // Enable outputs (columns)
    AT91C_BASE_PIOC->PIO_SODR = (1 << PIN_KB_1) | (1 << PIN_KB_3) | (1 << PIN_KB_5); // Set HIGH initially

    // Configure inputs (rows)
    AT91C_BASE_PIOC->PIO_ODR = (1 << PIN_KB_2) | (1 << PIN_KB_4) | (1 << PIN_KB_6) | (1 << PIN_KB_7);
    // Disable pull-up resistors
    AT91C_BASE_PIOC->PIO_PPUDR = (1 << PIN_KB_1) | (1 << PIN_KB_3) | (1 << PIN_KB_5) |                  // Configure outputs (columns)
                                 (1 << PIN_KB_2) | (1 << PIN_KB_4) | (1 << PIN_KB_6) | (1 << PIN_KB_7); // Configure inputs (rows)

    // Enable input filters for debouncing
    AT91C_BASE_PIOC->PIO_IFER = (1 << PIN_KB_2) | (1 << PIN_KB_4) | (1 << PIN_KB_6) | (1 << PIN_KB_7);
}

// Reconfigure keypad pins (needed after LCD operations that change pin directions)
void reconfigureKeypadPins(void)
{
    // Configure outputs (columns)
    AT91C_BASE_PIOC->PIO_OER = (1 << PIN_KB_1) | (1 << PIN_KB_3) | (1 << PIN_KB_5);  // Enable outputs (columns)
    AT91C_BASE_PIOC->PIO_SODR = (1 << PIN_KB_1) | (1 << PIN_KB_3) | (1 << PIN_KB_5); // Set HIGH initially

    // Configure inputs (rows)
    AT91C_BASE_PIOC->PIO_ODR = (1 << PIN_KB_2) | (1 << PIN_KB_4) | (1 << PIN_KB_6) | (1 << PIN_KB_7);
}

// Read the keypad state
void readKeypad(unsigned int *keypadValue)
{
    int value = 0;
    int columns[] = {PIN_KB_3, PIN_KB_1, PIN_KB_5};        // Columns order (based on keypad sheet)
    int rows[] = {PIN_KB_2, PIN_KB_7, PIN_KB_6, PIN_KB_4}; // Rows order (based on keypad sheet)

    // Reconfigure pins in case LCD changed them
    reconfigureKeypadPins();

    for (int col = 0; col < 3; col++)
    {
        // Set all columns high except the current one
        AT91C_BASE_PIOC->PIO_SODR = (1 << PIN_KB_1) | (1 << PIN_KB_3) | (1 << PIN_KB_5);
        AT91C_BASE_PIOC->PIO_CODR = (1 << columns[col]);

        // Small delay for signal stabilization
        for (volatile int i = 0; i < 100; i++)
            ;

        // Read rows
        for (int row = 0; row < 4; row++)
        {
            if ((AT91C_BASE_PIOC->PIO_PDSR & (1 << rows[row])) == 0)
            {
                // Key is pressed
                value = (row * 3) + (col + 1); // Calculate key value (1-12)
                *keypadValue = value;
                return; // Exit after first detected key press
            }
        }
    }
    *keypadValue = 0; // No key pressed
}
