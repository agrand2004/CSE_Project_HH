#ifndef TEMP_SENSOR_FUNCTION_H
#define TEMP_SENSOR_FUNCTION_H

#include <stdint.h>

// Global flag for interrupt (external variable)
extern volatile uint8_t tempDataReady;

// Temperature sensor initialization
void tempSensorInit(void);

// Start a temperature sensor reading (non-blocking)
void readTempSensor(void);

// Get temperature value after data is ready
float getTemperature(void);

// Interrupt handler (declared for reference)
void TC0_Handler(void);

#endif