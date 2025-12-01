#include "system_sam3x.h"
#include "at91sam3x8.h"
#include "core_cm3.h"
#include "photoresistor_function.h"

#define PIN_PHOTORESISTOR 4 // A4 -> pin connected to photoresistor

float adcToVoltage(uint16_t adcValue) {
    return ((float)adcValue * 3.3f) / 4095.0f;
}

void photoresistorInit(void)
{
    AT91C_BASE_PMC->PMC_PCER1 = (1 << (AT91C_ID_ADCC - 32));
    AT91C_BASE_PMC->PMC_PCER = (1 << AT91C_ID_PIOA);

    AT91C_BASE_PIOA->PIO_PER = (1 << PIN_PHOTORESISTOR); // Enable PIO control

    uint16_t prescaler = 2; // Prescaler value for ADC clock (MCK/8 = 10.5MHz at 84MHz MCK)
    AT91C_BASE_ADCC->ADC_MR = (prescaler << 8); // Set prescaler

    AT91C_BASE_ADCC->ADC_CHER = (1 << PHOTORESISTOR_CHANNEL);
}

float readADCChannel(uint8_t channel)
{
    // 1. Select and enable the channel in ADC_CHER
    AT91C_BASE_ADCC->ADC_CHER = (1 << channel);

    // 2. Start ADC conversion in ADC_CR
    AT91C_BASE_ADCC->ADC_CR = AT91C_ADC_START;

    // 3. Wait until DRDY (Data Ready) is set in ADC_SR
    while (!(AT91C_BASE_ADCC->ADC_SR & AT91C_ADC_DRDY))
    {
        // Wait for conversion to complete
    }

    // 4. Read value from ADC_LCDR (Last Converted Data Register)
    // The LCDR contains the last converted value (12-bit, 0-4095)
    uint16_t adcValue = AT91C_BASE_ADCC->ADC_LCDR & 0x00000FFF; // Mask to get 12-bit value
    return adcToVoltage(adcValue);
}

float readPhotoresistor(void)
{
    return readADCChannel(PHOTORESISTOR_CHANNEL);
}
