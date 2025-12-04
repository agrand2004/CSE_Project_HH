#ifndef UTILS_H
#define UTILS_H

#include <stdint.h>
#include <stdio.h>
#include "datetime.h"

void floatToString(float value, char *buffer);
void delay(unsigned int us);

// Convert timestamp to time string format "hh:mm:ss"
void timeToString(DateTime dateTime, char *buffer, int bufferSize);

// Convert timestamp to date string format "DD/MM/YYYY"
void dateToString(DateTime dateTime, char *buffer, int bufferSize);

#endif // UTILS_H
