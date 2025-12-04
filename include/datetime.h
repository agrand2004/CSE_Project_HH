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

// ask for the user to write the date and time
void buildDateTime(void); //TODO

// Add one second to the current date/time
void addOneSecond(void);

#endif