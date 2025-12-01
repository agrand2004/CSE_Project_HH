#include <stdint.h>

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