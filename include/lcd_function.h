#ifndef LCD_FUNCTION_H
#define LCD_FUNCTION_H

// Delay functions (defined in main.c)
void delay(unsigned int us);

// LCD data bus configuration
void lcdDataBusOutput(void);
void lcdDataBusInput(void);
void reconfigureLcdPins(void);

// LCD low-level operations
void lcdWriteBus(unsigned char data);
unsigned char lcdReadStatus(void);
void lcdWaitReady(void);

// LCD data and command writing
void lcdWriteData(unsigned char data);
void lcdWriteCommand(unsigned char cmd);

// LCD initialization and control
void lcdReset(void);
void lcdSetAddress(unsigned int address);
void lcdInit(void);

// LCD text operations
void lcdClearText(void);
void lcdGotoXY(unsigned char x, unsigned char y);
void lcdPutc(char c);
void lcdPuts(const char *str);
void lcdPrintAt(unsigned char x, unsigned char y, const char *str);
void lcdClearLine(unsigned char line);
void lcdFinishLine(unsigned char line, int currentPos);

#endif // LCD_FUNCTION_H
