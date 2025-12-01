#ifndef PHOTORESISTOR_FUNCTION_H
#define PHOTORESISTOR_FUNCTION_H

#include <stdint.h>

// ADC channel for photoresistor (adjust based on your hardware connection)
#define PHOTORESISTOR_CHANNEL 2

// Initialize ADC for photoresistor reading
void photoresistorInit(void);

// Read value from photoresistor (blocking)
float readPhotoresistor(void);

// Read value from a specific ADC channel
float readADCChannel(uint8_t channel);

#endif
