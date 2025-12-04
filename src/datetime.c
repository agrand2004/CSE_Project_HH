#include "greenhouse.h"
#include "datetime.h"

uint8_t monthDayCnt[] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
uint8_t monthDayCntLeap[] = {31, 29, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};

// Helper function to check if a year is a leap year
static Boolean isLeapYear(uint16_t year)
{
    if (year % 400 == 0)
        return TRUE;
    if (year % 100 == 0)
        return FALSE;
    if (year % 4 == 0)
        return TRUE;
    return FALSE;
}

// Helper function to get the number of days in a given month
static uint8_t getDaysInMonth(uint8_t month, uint16_t year)
{
    if (month <= 0 || month > 12)
        return 0;

    if (isLeapYear(year))
        return monthDayCntLeap[month - 1];
    else
        return monthDayCnt[month - 1];
}

// Add one second to the current date/time
void addOneSecond(void)
{
    // Increment seconds
    greenhouse.currentDateTime.seconds++;

    // Check if we need to carry over to minutes
    if (greenhouse.currentDateTime.seconds >= 60)
    {
        greenhouse.currentDateTime.seconds = 0;
        greenhouse.currentDateTime.minutes++;

        // Check if we need to carry over to hours
        if (greenhouse.currentDateTime.minutes >= 60)
        {
            greenhouse.currentDateTime.minutes = 0;
            greenhouse.currentDateTime.hours++;

            // Check if we need to carry over to days
            if (greenhouse.currentDateTime.hours >= 24)
            {
                greenhouse.currentDateTime.hours = 0;
                greenhouse.currentDateTime.day++;

                // Check if we need to carry over to months
                uint8_t daysInMonth = getDaysInMonth(greenhouse.currentDateTime.month, greenhouse.currentDateTime.year);
                if (greenhouse.currentDateTime.day > daysInMonth)
                {
                    greenhouse.currentDateTime.day = 1;
                    greenhouse.currentDateTime.month++;

                    // Check if we need to carry over to years
                    if (greenhouse.currentDateTime.month > 12)
                    {
                        greenhouse.currentDateTime.month = 1;
                        greenhouse.currentDateTime.year++;
                    }
                }
            }
        }
    }
}

// Function to update the date and time of the greenhouse
// Called by SysTick every second
void updateDateTime(void)
{
    if (greenhouse.inTimestampConfiguration)
    {
        return; // Do not update if in configuration mode
    }

    addOneSecond();
}