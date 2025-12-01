#include "system_sam3x.h"
#include "at91sam3x8.h"
#include "core_cm3.h"
#include "pins.h"
#include "lcd_function.h"

void lcdDataBusOutput(void)
{
    // DIR = 0 : A->B (MCU to LCD)
    AT91C_BASE_PIOC->PIO_CODR = (1 << PIN_74HC245_LCD_DIR);

    // Configure DB0-DB7 in output
    AT91C_BASE_PIOC->PIO_OER = (1 << PIN_LCD_DB0) | (1 << PIN_LCD_DB1) |
                               (1 << PIN_LCD_DB2) | (1 << PIN_LCD_DB3) |
                               (1 << PIN_LCD_DB4) | (1 << PIN_LCD_DB5) |
                               (1 << PIN_LCD_DB6) | (1 << PIN_LCD_DB7);
}

// Configure the data bus in input (for reading)
void lcdDataBusInput(void)
{
    // DIR = 1 : B->A (LCD to MCU)
    AT91C_BASE_PIOC->PIO_SODR = (1 << PIN_74HC245_LCD_DIR);

    // Configure DB0-DB7 as input
    AT91C_BASE_PIOC->PIO_ODR = (1 << PIN_LCD_DB0) | (1 << PIN_LCD_DB1) |
                               (1 << PIN_LCD_DB2) | (1 << PIN_LCD_DB3) |
                               (1 << PIN_LCD_DB4) | (1 << PIN_LCD_DB5) |
                               (1 << PIN_LCD_DB6) | (1 << PIN_LCD_DB7);
}

// Reconfigure LCD control pins (needed after keypad operations)
void reconfigureLcdPins(void)
{
    // Enable PIO control for LCD pins
    AT91C_BASE_PIOC->PIO_PER = (1 << PIN_74HC245_LCD_G) | (1 << PIN_74HC245_LCD_DIR) |
                               (1 << PIN_LCD_DB0) | (1 << PIN_LCD_DB1) | (1 << PIN_LCD_DB2) | (1 << PIN_LCD_DB3) |
                               (1 << PIN_LCD_DB4) | (1 << PIN_LCD_DB5) | (1 << PIN_LCD_DB6) | (1 << PIN_LCD_DB7) |
                               (1 << PIN_LCD_CD) | (1 << PIN_LCD_CE) |
                               (1 << PIN_LCD_WR) | (1 << PIN_LCD_RD) |
                               (1 << PIN_LCD_FS) | (1 << PIN_LCD_RV);
    AT91C_BASE_PIOD->PIO_PER = (1 << PIN_LCD_RESET);

    // Configure LCD control pins as outputs
    AT91C_BASE_PIOC->PIO_OER = (1 << PIN_74HC245_LCD_G) | (1 << PIN_74HC245_LCD_DIR) |
                               (1 << PIN_LCD_CD) | (1 << PIN_LCD_CE) |
                               (1 << PIN_LCD_WR) | (1 << PIN_LCD_RD) |
                               (1 << PIN_LCD_FS) | (1 << PIN_LCD_RV);
    AT91C_BASE_PIOD->PIO_OER = (1 << PIN_LCD_RESET);
}

void lcdWriteBus(unsigned char data)
{
    // Extract each bit and set/clear corresponding pins
    if (data & 0x01)
        AT91C_BASE_PIOC->PIO_SODR = (1 << PIN_LCD_DB0);
    else
        AT91C_BASE_PIOC->PIO_CODR = (1 << PIN_LCD_DB0);

    if (data & 0x02)
        AT91C_BASE_PIOC->PIO_SODR = (1 << PIN_LCD_DB1);
    else
        AT91C_BASE_PIOC->PIO_CODR = (1 << PIN_LCD_DB1);

    if (data & 0x04)
        AT91C_BASE_PIOC->PIO_SODR = (1 << PIN_LCD_DB2);
    else
        AT91C_BASE_PIOC->PIO_CODR = (1 << PIN_LCD_DB2);

    if (data & 0x08)
        AT91C_BASE_PIOC->PIO_SODR = (1 << PIN_LCD_DB3);
    else
        AT91C_BASE_PIOC->PIO_CODR = (1 << PIN_LCD_DB3);

    if (data & 0x10)
        AT91C_BASE_PIOC->PIO_SODR = (1 << PIN_LCD_DB4);
    else
        AT91C_BASE_PIOC->PIO_CODR = (1 << PIN_LCD_DB4);

    if (data & 0x20)
        AT91C_BASE_PIOC->PIO_SODR = (1 << PIN_LCD_DB5);
    else
        AT91C_BASE_PIOC->PIO_CODR = (1 << PIN_LCD_DB5);

    if (data & 0x40)
        AT91C_BASE_PIOC->PIO_SODR = (1 << PIN_LCD_DB6);
    else
        AT91C_BASE_PIOC->PIO_CODR = (1 << PIN_LCD_DB6);

    if (data & 0x80)
        AT91C_BASE_PIOC->PIO_SODR = (1 << PIN_LCD_DB7);
    else
        AT91C_BASE_PIOC->PIO_CODR = (1 << PIN_LCD_DB7);
}

// Read LCD status register
unsigned char lcdReadStatus(void)
{
    unsigned char status;
    unsigned int pdsr;

    // Reconfigure pins in case keypad changed them
    reconfigureLcdPins();

    // 1. Make databus as input
    lcdDataBusInput();

    // 3. Clear/enable output (74HC245: G = 0 to enable)
    AT91C_BASE_PIOC->PIO_CODR = (1 << PIN_74HC245_LCD_G);

    // 4. Set C/D (C/D = 1 for command/status)
    AT91C_BASE_PIOC->PIO_SODR = (1 << PIN_LCD_CD);

    // 5. Clear chip select display (CE = 0 to select)
    AT91C_BASE_PIOC->PIO_CODR = (1 << PIN_LCD_CE);

    // 6. Clear read display (RD = 0 to read)
    AT91C_BASE_PIOC->PIO_CODR = (1 << PIN_LCD_RD);

    // 7. Make a delay
    delay(100);

    // 8. Read data bus and save it in temp
    pdsr = AT91C_BASE_PIOC->PIO_PDSR;
    status = 0;
    if (pdsr & (1 << PIN_LCD_DB0))
        status |= 0x01;
    if (pdsr & (1 << PIN_LCD_DB1))
        status |= 0x02;
    if (pdsr & (1 << PIN_LCD_DB2))
        status |= 0x04;
    if (pdsr & (1 << PIN_LCD_DB3))
        status |= 0x08;
    if (pdsr & (1 << PIN_LCD_DB4))
        status |= 0x10;
    if (pdsr & (1 << PIN_LCD_DB5))
        status |= 0x20;
    if (pdsr & (1 << PIN_LCD_DB6))
        status |= 0x40;
    if (pdsr & (1 << PIN_LCD_DB7))
        status |= 0x80;

    // 9. Set chip select display (CE = 1 to deselect)
    AT91C_BASE_PIOC->PIO_SODR = (1 << PIN_LCD_CE);

    // 10. Set read display (RD = 1 to end read)
    AT91C_BASE_PIOC->PIO_SODR = (1 << PIN_LCD_RD);

    // 11. Disable output (74HC245: G = 1 to disable)
    AT91C_BASE_PIOC->PIO_SODR = (1 << PIN_74HC245_LCD_G);

    // 12. Make databus as output
    lcdDataBusOutput();

    // 13. Return (Temp)
    return status;
}

void lcdWaitReady(void)
{
    unsigned char status = lcdReadStatus();
    while (((status & (STATUS_STA0 | STATUS_STA1)) != (STATUS_STA0 | STATUS_STA1)))
    {
        delay(10);
        status = lcdReadStatus();
    }
}

// Write data to LCD
void lcdWriteData(unsigned char data)
{
    // Reconfigure pins in case keypad changed them
    reconfigureLcdPins();

    // 1. Wait until Read_Status_Display returns an OK
    lcdWaitReady();

    // 2. Clear databus (set all data pins to 0)
    AT91C_BASE_PIOC->PIO_CODR = (1 << PIN_LCD_DB0) | (1 << PIN_LCD_DB1) |
                                (1 << PIN_LCD_DB2) | (1 << PIN_LCD_DB3) |
                                (1 << PIN_LCD_DB4) | (1 << PIN_LCD_DB5) |
                                (1 << PIN_LCD_DB6) | (1 << PIN_LCD_DB7);

    // 3. Set Data to databus
    lcdWriteBus(data);

    // 4. Set dir as output (74HC245: DIR = 0 for A->B, MCU to LCD)
    lcdDataBusOutput();

    // 5. Enable output (74HC245: G = 0 to enable)
    AT91C_BASE_PIOC->PIO_CODR = (1 << PIN_74HC245_LCD_G);

    // 7. Clear C/D signal (0 = Data)
    AT91C_BASE_PIOC->PIO_CODR = (1 << PIN_LCD_CD);

    // 8. Clear chip select display (CE = 0)
    AT91C_BASE_PIOC->PIO_CODR = (1 << PIN_LCD_CE);

    // 9. Clear write display (WR = 0)
    AT91C_BASE_PIOC->PIO_CODR = (1 << PIN_LCD_WR);

    // 10. Make a Delay
    delay(100);

    // 11. Set chip enable display (CE = 1)
    AT91C_BASE_PIOC->PIO_SODR = (1 << PIN_LCD_CE);

    // 12. Set write display (WR = 1)
    AT91C_BASE_PIOC->PIO_SODR = (1 << PIN_LCD_WR);

    // 13. Disable output (74HC245: G = 1 to disable)
    AT91C_BASE_PIOC->PIO_SODR = (1 << PIN_74HC245_LCD_G);

    // 14. Make databus as input
    AT91C_BASE_PIOC->PIO_ODR = (1 << PIN_LCD_DB0) | (1 << PIN_LCD_DB1) |
                               (1 << PIN_LCD_DB2) | (1 << PIN_LCD_DB3) |
                               (1 << PIN_LCD_DB4) | (1 << PIN_LCD_DB5) |
                               (1 << PIN_LCD_DB6) | (1 << PIN_LCD_DB7);
}

// Write command to LCD
void lcdWriteCommand(unsigned char cmd)
{
    // Reconfigure pins in case keypad changed them
    reconfigureLcdPins();

    // 1. Wait until Read_Status_Display returns an OK
    lcdWaitReady();

    // 2. Clear databus (set all data pins to 0)
    AT91C_BASE_PIOC->PIO_CODR = (1 << PIN_LCD_DB0) | (1 << PIN_LCD_DB1) |
                                (1 << PIN_LCD_DB2) | (1 << PIN_LCD_DB3) |
                                (1 << PIN_LCD_DB4) | (1 << PIN_LCD_DB5) |
                                (1 << PIN_LCD_DB6) | (1 << PIN_LCD_DB7);

    // 3. Set Command to databus
    lcdWriteBus(cmd);

    lcdDataBusOutput();

    // 5. Enable output (74HC245: G = 0 to enable)
    AT91C_BASE_PIOC->PIO_CODR = (1 << PIN_74HC245_LCD_G);

    // 7. Set C/D signal High (1 = Command)
    AT91C_BASE_PIOC->PIO_SODR = (1 << PIN_LCD_CD);

    // 8. Clear chip select display (CE = 0)
    AT91C_BASE_PIOC->PIO_CODR = (1 << PIN_LCD_CE);

    // 9. Clear write display (WR = 0)
    AT91C_BASE_PIOC->PIO_CODR = (1 << PIN_LCD_WR);

    // 10. Make a Delay
    delay(100);

    // 11. Set chip enable display (CE = 1)
    AT91C_BASE_PIOC->PIO_SODR = (1 << PIN_LCD_CE);

    // 12. Set write display (WR = 1)
    AT91C_BASE_PIOC->PIO_SODR = (1 << PIN_LCD_WR);

    // 13. Disable output (74HC245: G = 1 to disable)
    AT91C_BASE_PIOC->PIO_SODR = (1 << PIN_74HC245_LCD_G);

    // 14. Make databus as input
    AT91C_BASE_PIOC->PIO_ODR = (1 << PIN_LCD_DB0) | (1 << PIN_LCD_DB1) |
                               (1 << PIN_LCD_DB2) | (1 << PIN_LCD_DB3) |
                               (1 << PIN_LCD_DB4) | (1 << PIN_LCD_DB5) |
                               (1 << PIN_LCD_DB6) | (1 << PIN_LCD_DB7);
}

// Hardware reset of LCD
void lcdReset(void)
{
    AT91C_BASE_PIOD->PIO_CODR = (1 << PIN_LCD_RESET); // RESET LOW
    delay(50);                                        // Hold reset for at least 50ms
    AT91C_BASE_PIOD->PIO_SODR = (1 << PIN_LCD_RESET); // RESET HIGH
    delay(50);                                        // Wait for LCD to wake up after reset
}

// Set memory address
void lcdSetAddress(unsigned int address)
{
    lcdWriteData(address & 0xFF);        // Low byte
    lcdWriteData((address >> 8) & 0xFF); // High byte
    lcdWriteCommand(CMD_SET_ADDRESS_POINTER);
}

// Initialize LCD
void lcdInit(void)
{
    // Enable peripheral clock for PIOC and PIOD
    AT91C_BASE_PMC->PMC_PCER = (1 << AT91C_ID_PIOC) | (1 << AT91C_ID_PIOD);

    // Enable PIO control for LCD pins on Port C
    AT91C_BASE_PIOC->PIO_PER = (1 << PIN_74HC245_LCD_G) | (1 << PIN_74HC245_LCD_DIR) |
                               (1 << PIN_LCD_DB0) | (1 << PIN_LCD_DB1) | (1 << PIN_LCD_DB2) | (1 << PIN_LCD_DB3) |
                               (1 << PIN_LCD_DB4) | (1 << PIN_LCD_DB5) | (1 << PIN_LCD_DB6) | (1 << PIN_LCD_DB7) |
                               (1 << PIN_LCD_WR) | (1 << PIN_LCD_RD) | (1 << PIN_LCD_CE) | (1 << PIN_LCD_CD) |
                               (1 << PIN_LCD_FS) | (1 << PIN_LCD_RV);

    // Enable PIO control for RESET on Port D
    AT91C_BASE_PIOD->PIO_PER = (1 << PIN_LCD_RESET);

    // Configure all control pins as output
    AT91C_BASE_PIOC->PIO_OER = (1 << PIN_74HC245_LCD_G) | (1 << PIN_74HC245_LCD_DIR) |
                               (1 << PIN_LCD_WR) | (1 << PIN_LCD_RD) | (1 << PIN_LCD_CE) |
                               (1 << PIN_LCD_CD) | (1 << PIN_LCD_FS) | (1 << PIN_LCD_RV);
    AT91C_BASE_PIOD->PIO_OER = (1 << PIN_LCD_RESET);

    // Initial state: inactive signals (HIGH for WR, RD, CE, CD)
    AT91C_BASE_PIOC->PIO_SODR = (1 << PIN_LCD_WR) | (1 << PIN_LCD_RD) |
                                (1 << PIN_LCD_CE) | (1 << PIN_LCD_CD);

    // RESET LOW initially
    AT91C_BASE_PIOD->PIO_CODR = (1 << PIN_LCD_RESET);

    // Enable 74HC245 for LCD (G = 0)
    AT91C_BASE_PIOC->PIO_CODR = (1 << PIN_74HC245_LCD_G);

    // FS = 0 (8x8 font), RV = 0 (normal display)
    AT91C_BASE_PIOC->PIO_CODR = (1 << PIN_LCD_FS) | (1 << PIN_LCD_RV);

    // Data bus as output by default
    lcdDataBusOutput();

    // Clear Reset display
    AT91C_BASE_PIOD->PIO_CODR = (1 << PIN_LCD_RESET);

    // Make a Delay
    delay(100);

    // Set Reset display
    AT91C_BASE_PIOD->PIO_SODR = (1 << PIN_LCD_RESET);

    // Set text home address (0x0000)
    lcdWriteData(0x00);
    lcdWriteData(0x00);
    lcdWriteCommand(0x40); // Set text home address

    // Set graphic home address (0x4000)
    lcdWriteData(0x00);
    lcdWriteData(0x40);
    lcdWriteCommand(0x42); // Set graphic home address

    // Set text area (0x001E = 30 columns)
    lcdWriteData(0x1E);
    lcdWriteData(0x00);
    lcdWriteCommand(0x41); // Set text area

    // Set graphic area (0x001E = 30 columns)
    lcdWriteData(0x1E);
    lcdWriteData(0x00);
    lcdWriteCommand(0x43); // Set graphic area

    lcdWriteCommand(0x80); // text mode
    lcdWriteCommand(0x94); // Text on graphic off
}

// Clear screen in text mode
void lcdClearText(void)
{
    lcdSetAddress(TEXT_HOME);
    lcdWriteCommand(CMD_AUTO_WRITE);

    for (unsigned int i = 0; i < (LCD_COLS * LCD_ROWS); i++)
    {
        lcdWriteData(0x00); // T6963C code for space (ASCII 0x20 -> T6963C 0x00)
        lcdWaitReady();
    }

    lcdWriteCommand(CMD_AUTO_RESET);
}

// Position cursor
void lcdGotoXY(unsigned char x, unsigned char y)
{
    unsigned int address = TEXT_HOME + (y * TEXT_AREA) + x;
    lcdSetAddress(address);
}

// Display a character
void lcdPutc(char c)
{
    /*
     * T6963C Character Code Mapping
     * =============================
     * ASCII Standard -> T6963C Code (formula: T6963C = ASCII - 0x20)
     *
     * Common characters:
     * ' ' (0x20) -> 0x00 | '0' (0x30) -> 0x10 | '@' (0x40) -> 0x20 | 'P' (0x50) -> 0x30
     * '!' (0x21) -> 0x01 | '1' (0x31) -> 0x11 | 'A' (0x41) -> 0x21 | 'Q' (0x51) -> 0x31
     * '"' (0x22) -> 0x02 | '2' (0x32) -> 0x12 | 'B' (0x42) -> 0x22 | 'R' (0x52) -> 0x32
     * '#' (0x23) -> 0x03 | '3' (0x33) -> 0x13 | 'C' (0x43) -> 0x23 | 'S' (0x53) -> 0x33
     * '$' (0x24) -> 0x04 | '4' (0x34) -> 0x14 | 'D' (0x44) -> 0x24 | 'T' (0x54) -> 0x34
     * '%' (0x25) -> 0x05 | '5' (0x35) -> 0x15 | 'E' (0x45) -> 0x25 | 'U' (0x55) -> 0x35
     * '&' (0x26) -> 0x06 | '6' (0x36) -> 0x16 | 'F' (0x46) -> 0x26 | 'V' (0x56) -> 0x36
     * ''' (0x27) -> 0x07 | '7' (0x37) -> 0x17 | 'G' (0x47) -> 0x27 | 'W' (0x57) -> 0x37
     * '(' (0x28) -> 0x08 | '8' (0x38) -> 0x18 | 'H' (0x48) -> 0x28 | 'X' (0x58) -> 0x38
     * ')' (0x29) -> 0x09 | '9' (0x39) -> 0x19 | 'I' (0x49) -> 0x29 | 'Y' (0x59) -> 0x39
     * '*' (0x2A) -> 0x0A | ':' (0x3A) -> 0x1A | 'J' (0x4A) -> 0x2A | 'Z' (0x5A) -> 0x3A
     * '+' (0x2B) -> 0x0B | ';' (0x3B) -> 0x1B | 'K' (0x4B) -> 0x2B | '[' (0x5B) -> 0x3B
     * ',' (0x2C) -> 0x0C | '<' (0x3C) -> 0x1C | 'L' (0x4C) -> 0x2C | '\' (0x5C) -> 0x3C
     * '-' (0x2D) -> 0x0D | '=' (0x3D) -> 0x1D | 'M' (0x4D) -> 0x2D | ']' (0x5D) -> 0x3D
     * '.' (0x2E) -> 0x0E | '>' (0x3E) -> 0x1E | 'N' (0x4E) -> 0x2E | '^' (0x5E) -> 0x3E
     * '/' (0x2F) -> 0x0F | '?' (0x3F) -> 0x1F | 'O' (0x4F) -> 0x2F | '_' (0x5F) -> 0x3F
     *
     * Lowercase letters:
     * '`' (0x60) -> 0x40 | 'p' (0x70) -> 0x50
     * 'a' (0x61) -> 0x41 | 'q' (0x71) -> 0x51
     * 'b' (0x62) -> 0x42 | 'r' (0x72) -> 0x52
     * 'c' (0x63) -> 0x43 | 's' (0x73) -> 0x53
     * 'd' (0x64) -> 0x44 | 't' (0x74) -> 0x54
     * 'e' (0x65) -> 0x45 | 'u' (0x75) -> 0x55
     * 'f' (0x66) -> 0x46 | 'v' (0x76) -> 0x56
     * 'g' (0x67) -> 0x47 | 'w' (0x77) -> 0x57
     * 'h' (0x68) -> 0x48 | 'x' (0x78) -> 0x58
     * 'i' (0x69) -> 0x49 | 'y' (0x79) -> 0x59
     * 'j' (0x6A) -> 0x4A | 'z' (0x7A) -> 0x5A
     * 'k' (0x6B) -> 0x4B | '{' (0x7B) -> 0x5B
     * 'l' (0x6C) -> 0x4C | '|' (0x7C) -> 0x5C
     * 'm' (0x6D) -> 0x4D | '}' (0x7D) -> 0x5D
     * 'n' (0x6E) -> 0x4E | '~' (0x7E) -> 0x5E
     * 'o' (0x6F) -> 0x4F | DEL(0x7F) -> 0x5F
     */

    if (c >= 0x20 && c <= 0x7F)
    {
        lcdWriteData(c - 0x20);
        lcdWriteCommand(CMD_DATA_WRITE_INC);
    }
}

// Display a string
void lcdPuts(const char *str)
{
    while (*str)
    {
        lcdPutc(*str++);
    }
}

// Display a string at given position
void lcdPrintAt(unsigned char x, unsigned char y, const char *str)
{
    lcdGotoXY(x, y);
    lcdPuts(str);
}

void lcdClearLine(unsigned char line)
{
    lcdGotoXY(0, line);
    for (unsigned char i = 0; i < LCD_COLS; i++)
    {
        lcdPutc(' ');
    }
}