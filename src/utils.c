#include "utils.h"

// Helper function to convert integer to string
static int intToString(uint32_t value, char *buffer, int bufferSize)
{
    if (bufferSize <= 0)
        return 0;

    char temp[20];
    int i = 0;

    if (value == 0)
    {
        temp[i++] = '0';
    }
    else
    {
        while (value > 0 && i < 20)
        {
            temp[i++] = '0' + (value % 10);
            value /= 10;
        }
    }

    // Reverse the string
    int j = 0;
    while (i > 0 && j < bufferSize - 1)
    {
        buffer[j++] = temp[--i];
    }

    return j;
}

void floatToString(float value, char *buffer)
{
    int intPart = (int)value;
    int decPart = (int)((value - intPart) * 10);
    if (decPart < 0)
        decPart = -decPart;

    int i = 0;
    if (intPart < 0)
    {
        buffer[i++] = '-';
        intPart = -intPart;
    }

    if (intPart == 0)
    {
        buffer[i++] = '0';
    }
    else
    {
        char temp[10];
        int j = 0;
        while (intPart > 0)
        {
            temp[j++] = '0' + (intPart % 10);
            intPart /= 10;
        }
        while (j > 0)
        {
            buffer[i++] = temp[--j];
        }
    }

    buffer[i++] = '.';
    buffer[i++] = '0' + decPart;
    buffer[i] = '\0';
}

void delay(unsigned int us)
{
    for (int i = 0; i < us; i++)
        asm("nop");
}

// Convert timestamp to time string format "hh:mm:ss"
void timeToString(DateTime dateTime, char *buffer, int bufferSize)
{
    int written = 0;

    // Format: hh
    if (dateTime.hours < 10)
        buffer[written++] = '0';
    written += intToString(dateTime.hours, &buffer[written], bufferSize - written);

    buffer[written++] = ':';

    // Format: mm
    if (dateTime.minutes < 10)
        buffer[written++] = '0';
    written += intToString(dateTime.minutes, &buffer[written], bufferSize - written);

    buffer[written++] = ':';

    // Format: ss
    if (dateTime.seconds < 10)
        buffer[written++] = '0';
    written += intToString(dateTime.seconds, &buffer[written], bufferSize - written);
    buffer[written] = '\0';
}

// Convert timestamp to date string format "DD/MM/YYYY"
void dateToString(DateTime dateTime, char *buffer, int bufferSize)
{
    int written = 0;

    // Format: DD
    if (dateTime.day < 10)
        buffer[written++] = '0';
    written += intToString(dateTime.day, &buffer[written], bufferSize - written);

    buffer[written++] = '/';

    // Format: MM
    if (dateTime.month < 10)
        buffer[written++] = '0';
    written += intToString(dateTime.month, &buffer[written], bufferSize - written);

    buffer[written++] = '/';

    // Format: YYYY
    written += intToString(dateTime.year, &buffer[written], bufferSize - written);

    buffer[written] = '\0';
}