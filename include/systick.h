#ifndef SYSTICK_H
#define SYSTICK_H

#include <stdint.h>

// Initialize SysTick with the given tick count (reload value)
// 'ticks' is the number of core clock cycles per tick.
void initSysTick(uint32_t ticks);

#endif // SYSTICK_H
