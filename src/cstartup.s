/**************************************************
 *
 * Part one of the system initialization code, contains low-level
 * initialization, plain thumb variant.
 *
 * Adapted from IAR to GCC syntax for ARM Cortex-M3
 *
 **************************************************/

    .syntax unified
    .cpu cortex-m3
    .fpu softvfp
    .thumb

    .global Reset_Handler
    .extern main
    .extern SystemInit

    .section .text.Reset_Handler
    .weak Reset_Handler
    .type Reset_Handler, %function

Reset_Handler:
    /* Call SystemInit function */
    bl SystemInit
    
    /* Call main function */
    bl main
    
    /* Infinite loop if main returns */
_exit_loop:
    b _exit_loop

    .size Reset_Handler, .-Reset_Handler
