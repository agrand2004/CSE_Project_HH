#ifndef DATETIME_H
#define DATETIME_H

#include <stdint.h>
#include <stdio.h>

typedef struct
{
    uint8_t seconds;
    uint8_t minutes;
    uint8_t hours;
    uint8_t day;
    uint8_t month;
    uint16_t year;
} DateTime;

// Update the date and time of the greenhouse
void updateDateTime(void);

// Configure the date and time through user input
void configureDateTime(void);

// Add one second to the current date/time
void addOneSecond(void);

#endif