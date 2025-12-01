#include "system_sam3x.h"
#include "at91sam3x8.h"
#include "core_cm3.h"
#include "servomotor_function.h"

#define SERVO_PIN 17      // PIOB pin 17 (Arduino A8)
#define PWM_CHANNEL 1     // PWM channel 1
#define PWM_PERIOD 52500  // 20ms period (for 50Hz servo signal)
#define PWM_MIN_DUTY 2625 // 1ms pulse (0 degrees)
#define PWM_MAX_DUTY 6563 // 2.5ms pulse (180 degrees)

void servomotorInit(void)
{
    // 1. Enable clock for PWM (peripheral ID 36, in PMC_PCER1)
    AT91C_BASE_PMC->PMC_PCER1 = (1 << (AT91C_ID_PWMC - 32));

    // 2. Configure PIOB pin 17 for PWM
    // Disable PIO control, let peripheral control the pin
    AT91C_BASE_PIOB->PIO_PDR = (1 << SERVO_PIN);

    // Select peripheral B (PWM) for pin 17
    AT91C_BASE_PIOB->PIO_ABMR = (1 << SERVO_PIN);

    // 4. Enable PWM channel 1
    AT91C_BASE_PWMC->PWMC_ENA = (1 << PWM_CHANNEL);

    // 3. Configure PWM channel 1
    // Set clock divider: MCK / 32 (prescaler = 5)
    AT91C_BASE_PWMC_CH1->PWMC_CMR = 5;

    // Set period to 20ms (50Hz for servo)
    AT91C_BASE_PWMC_CH1->PWMC_CPRDR = PWM_PERIOD;

    // Set initial duty cycle to 1ms (0 degrees)
    AT91C_BASE_PWMC_CH1->PWMC_CDTYR = PWM_MIN_DUTY;
}

void servomotorWriteValue(int angle)
{
    // Map value (0-180 degrees) to duty cycle (1ms-2.5ms)
    // value: 0-180 -> duty: PWM_MIN_DUTY to PWM_MAX_DUTY
    uint32_t duty = PWM_MIN_DUTY + ((angle * (PWM_MAX_DUTY - PWM_MIN_DUTY)) / 180);

    // Update duty cycle
    AT91C_BASE_PWMC_CH1->PWMC_CDTYUPDR = duty;
}
