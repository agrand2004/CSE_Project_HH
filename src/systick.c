#include "system_sam3x.h"
#include "at91sam3x8.h"
#include "core_cm3.h"

#include "systick.h"
#include "temp_sensor_function.h"
#include "photoresistor_function.h"
#include "greenhouse.h"

static unsigned int tickCounterHalfSecond = 0;
static unsigned int tickCounter = 0;

// Extern variable updated by SysTick ISR and displayed in main
extern GreenHouse greenhouse;

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

// Function executed every SysTick interrupt (0.1s)
void SysTick_Handler(void)
{
    tickCounter++;
    tickCounterHalfSecond++;

    // Read sensor at 0.5 second interval (5 ticks of 0.1s)
    if (tickCounterHalfSecond >= 5)
    { // 0.5 second elapsed
        readTempSensor();
        greenhouse.photoValue = readPhotoresistor();
        tickCounterHalfSecond = 0; // Reset half-second counter
        greenhouse.needDisplayUpdate = TRUE; // Indicate display needs update
    }

    // Update date/time and reset counter at 1 second (10 ticks of 0.1s)
    if (tickCounter >= 10)
    { // 1 second elapsed
        tickCounter = 0;
        updateDateTime();
        greenhouse.needDisplayUpdate = TRUE; // Indicate display needs update
    }
}
