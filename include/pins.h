#ifndef PINS_H
#define PINS_H

// ------- Keypad Pins -------
// Port D
#define PIN_74HC245_KB_G 2 // Connected to G on 74HC245 -> pin 27 on arduino

// Port C
#define PIN_KB_1 9 // Output -> pin 41 on arduino
#define PIN_KB_2 5 // Input -> pin 37
#define PIN_KB_3 8 // Output -> pin 40
#define PIN_KB_4 4 // Input -> pin 36
#define PIN_KB_5 7 // Output -> pin 39
#define PIN_KB_6 3 // Input -> pin 35
#define PIN_KB_7 2 // Input -> pin 34

// ------- LCD Pins -------
// LCD data bus (using 74HC245)
// Port C
#define PIN_74HC245_LCD_G 12   // Connected to G on 74HC245 -> pin 51 on arduino
#define PIN_74HC245_LCD_DIR 13 // Connected to DIR on 74HC245 -> pin 50 on arduino

#define PIN_LCD_DB0 2 // pin34 - DB0 -> pin 11 on LCD connector
#define PIN_LCD_DB1 3 // pin35 - DB1 -> pin 12 on LCD connector
#define PIN_LCD_DB2 4 // pin36 - DB2 -> pin 13 on LCD connector
#define PIN_LCD_DB3 5 // pin37 - DB3 -> pin 14 on LCD connector
#define PIN_LCD_DB4 6 // pin38 - DB4 -> pin 15 on LCD connector
#define PIN_LCD_DB5 7 // pin39 - DB5 -> pin 16 on LCD connector
#define PIN_LCD_DB6 8 // pin40 - DB6 -> pin 17 on LCD connector
#define PIN_LCD_DB7 9 // pin41 - DB7 -> pin 18 on LCD connector

#define PIN_LCD_RV 19 // pin44 - RV -> pin 20 on LCD connector
#define PIN_LCD_FS 18 // pin45 - FS -> pin 19 on LCD connector
#define PIN_LCD_WR 17 // pin46 - WR -> pin 5 on LCD connector
#define PIN_LCD_RD 16 // pin47 - RD -> pin 6 on LCD connector
#define PIN_LCD_CE 15 // pin48 - CE -> pin 7 on LCD connector
#define PIN_LCD_CD 14 // pin49 - C/D -> pin 8 on LCD connector

// Port D
#define PIN_LCD_RESET 0 // pin25 - RESET -> pin 10 on LCD connector

// ------- LCD Status Bits -------
#define STATUS_STA0 0x01 // Check command status
#define STATUS_STA1 0x02 // Check data read/write status

// ------- LCD Dimensions -------
#define LCD_WIDTH 240
#define LCD_HEIGHT 128
#define LCD_COLS 30 // 240/8 = 30 columns in text mode
#define LCD_ROWS 16 // 128/8 = 16 rows in text mode

// ------- LCD Memory Areas -------
#define TEXT_HOME 0x0000
#define GRAPHIC_HOME 0x0400
#define TEXT_AREA LCD_COLS
#define GRAPHIC_AREA LCD_COLS

// ------- T6963C Commands -------
// Registers Setting (00100XXX)
#define CMD_SET_CURSOR_POINTER 0x21  // 00100001 - Set cursor pointer
#define CMD_SET_OFFSET_REGISTER 0x22 // 00100010 - Set Offset Register
#define CMD_SET_ADDRESS_POINTER 0x24 // 00100100 - Set Address pointer

// Set Control Word (01000XXX)
#define CMD_SET_TEXT_HOME_ADDRESS 0x40    // 01000000 - Set Text Home Address
#define CMD_SET_TEXT_AREA 0x41            // 01000001 - Set Text Area
#define CMD_SET_GRAPHIC_HOME_ADDRESS 0x42 // 01000010 - Set Graphic Home Address
#define CMD_SET_GRAPHIC_AREA 0x43         // 01000011 - Set Graphic Area

// Mode Set (1000XXXX)
#define CMD_MODE_OR 0x80             // 10000000 - OR mode
#define CMD_MODE_EXOR 0x81           // 10000001 - EXOR mode
#define CMD_MODE_AND 0x83            // 10000011 - AND mode
#define CMD_MODE_TEXT_ATTRIBUTE 0x84 // 10000100 - Text Attribute mode
#define CMD_MODE_INTERNAL_CG 0x80    // 1000XXXX - Internal CG ROM mode
#define CMD_MODE_EXTERNAL_CG 0x88    // 10001XXX - External CG RAM mode

// Display Mode (1001XXXX)
#define CMD_DISPLAY_OFF 0x90         // 10010000 - Display off
#define CMD_CURSOR_BLINK_OFF 0x92    // 1001XX10 - Cursor on, blink off
#define CMD_CURSOR_BLINK_ON 0x93     // 1001XX11 - Cursor on, blink on
#define CMD_TEXT_ON_GRAPHIC_OFF 0x94 // 100101XX - Text on, graphic off
#define CMD_TEXT_OFF_GRAPHIC_ON 0x98 // 100110XX - Text off, graphic on
#define CMD_TEXT_ON_GRAPHIC_ON 0x9C  // 100111XX - Text on, graphic on

// Cursor Pattern Select
#define CMD_CURSOR_1_LINE 0xA0 // 1-line cursor
#define CMD_CURSOR_2_LINE 0xA1 // 2-line cursor
#define CMD_CURSOR_3_LINE 0xA2 // 3-line cursor
#define CMD_CURSOR_4_LINE 0xA3 // 4-line cursor
#define CMD_CURSOR_5_LINE 0xA4 // 5-line cursor
#define CMD_CURSOR_6_LINE 0xA5 // 6-line cursor
#define CMD_CURSOR_7_LINE 0xA6 // 7-line cursor
#define CMD_CURSOR_8_LINE 0xA7 // 8-line cursor

// Data Read/Write
#define CMD_DATA_WRITE_INC 0xC0   // Data Write and Increment ADP
#define CMD_DATA_READ_INC 0xC1    // Data Read and Increment ADP
#define CMD_DATA_WRITE_DEC 0xC2   // Data Write and Decrement ADP
#define CMD_DATA_READ_DEC 0xC3    // Data Read and Decrement ADP
#define CMD_DATA_WRITE_NOVAR 0xC4 // Data Write and Non-variable ADP
#define CMD_DATA_READ_NOVAR 0xC5  // Data Read and Non-variable ADP

// Data auto Read/Write
#define CMD_AUTO_WRITE 0xB0 // Set Data Auto Write
#define CMD_AUTO_READ 0xB1  // Set Data Auto Read
#define CMD_AUTO_RESET 0xB2 // Auto Reset

// Screen Peek/Copy
#define CMD_SCREEN_PEEK 0xE0 // Screen Peek
#define CMD_SCREEN_COPY 0xE8 // Screen Copy

// Bit Set/Reset
#define CMD_BIT_RESET 0xF0 // Bit Reset
#define CMD_BIT_SET 0xF8   // Bit Set
#define CMD_BIT_0 0xF0     // Bit 0 (LSB)
#define CMD_BIT_1 0xF1     // Bit 1
#define CMD_BIT_2 0xF2     // Bit 2
#define CMD_BIT_3 0xF3     // Bit 3
#define CMD_BIT_4 0xF4     // Bit 4
#define CMD_BIT_5 0xF5     // Bit 5
#define CMD_BIT_6 0xF6     // Bit 6
#define CMD_BIT_7 0xF7     // Bit 7 (MSB)

#define CMD_SCREEN_REVERSE 0xD0 // Screen Reverse

#endif // PINS_H
