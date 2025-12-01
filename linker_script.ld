/* Linker script for Atmel SAM3X8E (Arduino Due) - SRAM execution
 * Based on IAR sam3x8-sram.icf for bare-metal development
 */

/* Memory Spaces Definitions */
MEMORY
{
  RAM (rwx) : ORIGIN = 0x20000000, LENGTH = 0x00010000  /* 64KB SRAM0 */
  RAM1 (rwx) : ORIGIN = 0x20080000, LENGTH = 0x00008000 /* 32KB SRAM1 */
}

/* Entry Point */
ENTRY(Reset_Handler)

/* Section Definitions */
SECTIONS
{
    /* Code and read-only data */
    .text :
    {
        . = ALIGN(4);
        _sfixed = .;
        KEEP(*(.vectors .vectors.*))
        *(.text .text.* .gnu.linkonce.t.*)
        *(.glue_7t) *(.glue_7)
        *(.rodata .rodata* .gnu.linkonce.r.*)
        *(.ARM.extab* .gnu.linkonce.armextab.*)
        
        /* Support C constructors, and C destructors in both user code
           and the C library. This also provides support for C++ code. */
        . = ALIGN(4);
        KEEP(*(.init))
        . = ALIGN(4);
        __preinit_array_start = .;
        KEEP (*(.preinit_array))
        __preinit_array_end = .;

        . = ALIGN(4);
        __init_array_start = .;
        KEEP (*(SORT(.init_array.*)))
        KEEP (*(.init_array))
        __init_array_end = .;

        . = ALIGN(0x4);
        KEEP (*crtbegin.o(.ctors))
        KEEP (*(EXCLUDE_FILE (*crtend.o) .ctors))
        KEEP (*(SORT(.ctors.*)))
        KEEP (*crtend.o(.ctors))

        . = ALIGN(4);
        KEEP(*(.fini))

        . = ALIGN(4);
        __fini_array_start = .;
        KEEP (*(.fini_array))
        KEEP (*(SORT(.fini_array.*)))
        __fini_array_end = .;

        KEEP (*crtbegin.o(.dtors))
        KEEP (*(EXCLUDE_FILE (*crtend.o) .dtors))
        KEEP (*(SORT(.dtors.*)))
        KEEP (*crtend.o(.dtors))

        . = ALIGN(4);
        _efixed = .;            /* End of text section */
    } > RAM

    /* .ARM.exidx is sorted, so has to go in its own output section.  */
    PROVIDE_HIDDEN (__exidx_start = .);
    .ARM.exidx :
    {
      *(.ARM.exidx* .gnu.linkonce.armexidx.*)
    } > RAM
    PROVIDE_HIDDEN (__exidx_end = .);

    . = ALIGN(4);
    _etext = .;

    /* Initialized data section - LMA (load address) is in RAM for SRAM execution */
    .data :
    {
        . = ALIGN(4);
        _sdata = .;
        _sidata = _sdata; /* For SRAM execution, load and run address are the same */
        *(.data .data.*)
        . = ALIGN(4);
        _edata = .;
    } > RAM

    /* Uninitialized data section */
    .bss (NOLOAD) :
    {
        . = ALIGN(4);
        _sbss = .;
        __bss_start__ = _sbss;
        *(.bss .bss.*)
        *(COMMON)
        . = ALIGN(4);
        _ebss = .;
        __bss_end__ = _ebss;
    } > RAM

    /* Stack section */
    .stack (NOLOAD):
    {
        . = ALIGN(8);
        _sstack = .;
        . = . + 0x1000; /* 4KB stack */
        . = ALIGN(8);
        _estack = .;
    } > RAM

    /* Heap section */
    .heap (NOLOAD):
    {
        . = ALIGN(4);
        _sheap = .;
        . = ORIGIN(RAM) + LENGTH(RAM) - 0x1000;
        _eheap = .;
    } > RAM

    . = ALIGN(4);
    _end = . ;
}
