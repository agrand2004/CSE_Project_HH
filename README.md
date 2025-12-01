# Arduino Due Bare-Metal Project with PlatformIO

This project is configured for bare-metal programming on the Arduino Due board (SAM3X8E microcontroller) using PlatformIO and J-Link debugger.

## Hardware Setup
- Arduino Due board
- J-Link debugger connected via SWD
- USB power to the board

## Prerequisites
1. Install PlatformIO extension in VS Code
2. Install J-Link software from SEGGER website: https://www.segger.com/downloads/jlink/
3. Make sure J-Link drivers are properly installed

## Project Structure
```
├── src/
│   ├── main.c              # Your application code
│   ├── cstartup.s          # Startup assembly code
│   ├── startup_sam3x.c     # C startup code
│   ├── system_sam3x.c      # System initialization (clock, watchdog)
│   └── exceptions.c        # Exception handlers
├── include/
│   ├── at91sam3x8.h        # SAM3X8E register definitions
│   ├── system_sam3x.h      # System initialization header
│   ├── exceptions.h        # Exception handler prototypes
│   └── core_cm*.h          # ARM Cortex-M3 core headers
├── platformio.ini          # PlatformIO configuration
└── linker_script.ld        # Linker script for SRAM execution

```

## Building the Project
In VS Code terminal:
```bash
pio run
```

Or use the PlatformIO sidebar → Build button

## Uploading to Board
```bash
pio run --target upload
```

Or use the PlatformIO sidebar → Upload button

## Debugging
1. Connect J-Link debugger to Arduino Due SWD pins
2. Power the board
3. Press F5 or go to Run → Start Debugging
4. Use breakpoints, watch variables, step through code

### Debug Features Available:
- Set breakpoints by clicking left of line numbers
- Watch variables in the Debug sidebar
- Step Over (F10), Step Into (F11), Step Out (Shift+F11)
- View registers and memory directly
- Similar functionality to IAR Embedded Workbench

## Code Structure
The `main.c` file follows the same pattern as in IAR tutorial:

```c
#include "system_sam3x.h"
#include "at91sam3x8.h"

void main(void) {
    int i = 0;
    
    SystemInit(); // Disables the Watchdog and setup the MCK
    
    while(1) {
        i++;
        // Your code here
    }
}
```

### SystemInit() Function
This function (defined in `system_sam3x.c`):
- Disables the watchdog timer
- Configures the master clock (MCK) to use the crystal oscillator
- Sets up proper clock speeds for the microcontroller

## Register Access
You can access hardware registers directly using the definitions in `at91sam3x8.h`.

Example - Reading PIOD Port Data Status Register:
```c
volatile unsigned int port_value = PIOD->PIO_PDSR;
```

## Memory Map (SRAM Execution)
- SRAM0: 0x20000000 - 0x2000FFFF (64KB)
- SRAM1: 0x20080000 - 0x20087FFF (32KB)

This configuration runs code from SRAM for faster debugging cycles.

## Differences from IAR Embedded Workbench
- **Build System**: Uses PlatformIO instead of IAR build system
- **Debugger**: Uses GDB with J-Link instead of IAR C-SPY debugger
- **Editor**: VS Code instead of IAR IDE
- **Functionality**: Nearly identical - breakpoints, watches, stepping all work the same

## Troubleshooting

### J-Link Not Found
- Install J-Link software from SEGGER
- Check USB connection
- Verify driver installation

### Build Errors
- Run `pio run -v` for verbose output
- Check that all files from resources folder are present

### Upload Fails
- Ensure J-Link is properly connected to SWD pins on Due
- Power cycle the board
- Check J-Link connection with `JLinkExe`

## Next Steps
Start adding your code in the `while(1)` loop in `main.c`. You can:
- Configure GPIO pins
- Set up timers
- Use peripherals (UART, SPI, I2C, etc.)
- Add interrupt handlers in `exceptions.c`

## Resources
- [SAM3X Datasheet](https://www.microchip.com/en-us/product/ATSAM3X8E)
- [Arduino Due Pinout](https://docs.arduino.cc/hardware/due)
- [PlatformIO Documentation](https://docs.platformio.org/)
