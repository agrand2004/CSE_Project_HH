#include "system_sam3x.h"
#include "at91sam3x8.h"
#include "core_cm3.h"
#include "pins.h"
#include "temp_sensor_function.h"

// Pin definitions for temperature sensor
#define TEMP_SENSOR_PIN 25 // PIN 2 Arduino = PB25

// Global flag for interrupt
volatile uint8_t tempDataReady = 0;

// External delay function from main.c
extern void delay(unsigned int us);

// Accurate millisecond delay using SysTick timer
// Note: This function temporarily disables SysTick interrupts during the delay
void delayMilliseconds(uint32_t ms)
{
    // System clock is 84MHz, so 1 millisecond = 84,000 cycles
    uint32_t cycles = ms * 84000;

    // Save current SysTick state
    uint32_t ctrl = SysTick->CTRL;
    uint32_t load = SysTick->LOAD;

    // SysTick max is 16,777,215 cycles (~199ms at 84MHz)
    // For delays > 199ms, we need to loop
    while (cycles > 0)
    {
        uint32_t delay_cycles = (cycles > 16777215) ? 16777215 : cycles;

        // Disable SysTick during our delay
        SysTick->CTRL = 0;

        // Set up for our delay
        SysTick->LOAD = delay_cycles - 1;
        SysTick->VAL = 0; // Clear current value

        // Start SysTick (no interrupt, just counting)
        SysTick->CTRL = SysTick_CTRL_ENABLE_Msk | SysTick_CTRL_CLKSOURCE_Msk;

        // Wait until COUNTFLAG is set (counter reached 0)
        while (!(SysTick->CTRL & SysTick_CTRL_COUNTFLAG_Msk))
            ;

        cycles -= delay_cycles;
    }

    // Restore original SysTick configuration
    SysTick->CTRL = 0;
    SysTick->LOAD = load;
    SysTick->VAL = 0;
    SysTick->CTRL = ctrl;
}

void tempSensorConfigOutputPin(void)
{
    // Configure the pin as output (for start pulse)
    AT91C_BASE_PIOB->PIO_PER = (1 << TEMP_SENSOR_PIN); // Enable PIO control
    AT91C_BASE_PIOB->PIO_OER = (1 << TEMP_SENSOR_PIN); // Set as output
}

void tempSensorConfigInputPin(void)
{
    // Configure the pin as input (for reading sensor response)
    AT91C_BASE_PIOB->PIO_PER = (1 << TEMP_SENSOR_PIN); // Enable PIO control
    AT91C_BASE_PIOB->PIO_ODR = (1 << TEMP_SENSOR_PIN); // Set as input
}

void resetTempSensor(void)
{
    tempSensorConfigOutputPin();
    AT91C_BASE_PIOB->PIO_SODR = (1 << TEMP_SENSOR_PIN);
    AT91C_BASE_PIOB->PIO_CODR = (1 << TEMP_SENSOR_PIN);
    // Treset must be between 4.6ms and 16.0ms
    // Using accurate millisecond delay
    delayMilliseconds(10); // 10ms reset pulse
    AT91C_BASE_PIOB->PIO_SODR = (1 << TEMP_SENSOR_PIN);
}

void tempSensorInit(void)
{
    // Enable clock for TC0 (peripheral ID 27) and PIOB (peripheral ID 12)
    AT91C_BASE_PMC->PMC_PCER = (1 << AT91C_ID_TC0) | (1 << AT91C_ID_PIOB);

    // Configure the PIO line for the pin (PIOB pin 25)
    tempSensorConfigOutputPin();

    // Configure Timer Counter 0
    // Select Timer_Clock1 (MCK/2) as TCCLK
    AT91C_BASE_TC0->TC_CMR = AT91C_TC_CLKS_TIMER_DIV1_CLOCK;

    // Enable the counter and software reset
    AT91C_BASE_TC0->TC_CCR = AT91C_TC_CLKEN | AT91C_TC_SWTRG;

    // Load counter to RA when TIOA falling edge (start pulse falling edge)
    AT91C_BASE_TC0->TC_CMR |= AT91C_TC_LDRA_FALLING;
    // Load counter to RB when TIOA rising edge (sensor response rising edge)
    AT91C_BASE_TC0->TC_CMR |= AT91C_TC_LDRB_RISING;

    resetTempSensor();

    // Enable NVIC interrupt for TC0 (IRQ 27)
    NVIC_EnableIRQ(AT91C_ID_TC0);
}

void readTempSensor(void)
{
    // 1. Create a start pulse
    // Temporarily take back control of the pin
    AT91C_BASE_PIOB->PIO_PER = (1 << TEMP_SENSOR_PIN);
    AT91C_BASE_PIOB->PIO_OER = (1 << TEMP_SENSOR_PIN);

    // Pulse low for tSTART (datasheet specifies min duration)
    AT91C_BASE_PIOB->PIO_CODR = (1 << TEMP_SENSOR_PIN);
    delay(25);

    // Set high
    AT91C_BASE_PIOB->PIO_SODR = (1 << TEMP_SENSOR_PIN);
    AT91C_BASE_PIOB->PIO_ODR = (1 << TEMP_SENSOR_PIN);


    // 2. Software reset of counter
    AT91C_BASE_TC0->TC_CCR = AT91C_TC_SWTRG;

    // 3. Clear old interrupts by reading status register
    volatile uint32_t status = AT91C_BASE_TC0->TC_SR;
    (void)status; // Avoid unused variable warning

    // 4. Enable interrupt for LDRBS
    AT91C_BASE_TC0->TC_IER = AT91C_TC_LDRBS;

    // 5. Reset global flag
    tempDataReady = 0;
}

// Interrupt handler for TC0
void TC0_Handler(void)
{
    // 1. Read status register to clear interrupt
    volatile uint32_t status = AT91C_BASE_TC0->TC_SR;
    (void)status; // Avoid unused variable warning

    // 2. Disable LDRBS interrupt
    AT91C_BASE_TC0->TC_IDR = AT91C_TC_LDRBS;

    // 3. Set global flag
    tempDataReady = 1;
}

// Function to calculate temperature from registers
float getTemperature(void)
{
    if (!tempDataReady)
    {
        return -999.0f; // Error value
    }

    // Read register values
    uint32_t ra = AT91C_BASE_TC0->TC_RA; // Time of falling edge (start pulse)
    uint32_t rb = AT91C_BASE_TC0->TC_RB; // Time of falling edge (sensor response)

    // Calculate tDx: timing delay in clock cycles
    uint32_t tDx_cycles = rb - ra;

    // Timer_Clock1 = MCK/2 (typically 84MHz/2 = 42MHz for Arduino Due)
    // 1 cycle = 1/42MHz ≈ 23.8ns = 0.0238µs
    uint32_t tDx_us = tDx_cycles * 0.0238f; // Convert to microseconds

    // Apply formula: T(°C) = [tDx(µs) / 5(µs/°K)] - 273.15°K
    float timeout_multiplier = 5.0f; // µs/°K
    float temperature = (tDx_us / timeout_multiplier) - 273.15f;
    return temperature;
}
