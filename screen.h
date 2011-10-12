
#ifndef __SCREEN_H__
#define __SCREEN_H__

#include "types.h"

// Write a char at the cursor's current position
void arch_putc(uint8_t c);
void arch_putc_color(uint8_t c, uint8_t color);

// Clear the screen and set the cursor to the top-left position
void arch_cls(void);

#endif // __SCREEN_H__
