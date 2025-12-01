#ifndef SERVOMOTOR_FUNCTION_H
#define SERVOMOTOR_FUNCTION_H

#include <stdint.h>

// Initialize servomotor (PWM configuration)
void servomotorInit(void);

// Write value to servomotor
// value: Position value (typically 0-180 degrees or duty cycle percentage)
void servomotorWriteValue(int value);

#endif
