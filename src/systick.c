#include "system_sam3x.h"
#include "at91sam3x8.h"
#include "core_cm3.h"

#include "systick.h"
#include "temp_sensor_function.h"
#include "photoresistor_function.h"

#define DELAY_MS 1000 // 1000ms delay for reading temperature

static unsigned int tickCounter = 0;

// Extern variable updated by SysTick ISR and displayed in main
extern float photoValue;

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
