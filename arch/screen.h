
#ifndef __SCREEN_H__
#define __SCREEN_H__

#include "types.h"

// Colors for the screen chars
// 0:black, 1:blue, 2:green, 3:cyan, 4:red, 5:magenta, 6:brown,
// 7:light grey, 8:dark grey, 9:light blue, a:light green,
// b:light cyan, c:light red, d:light magneta, e: light brown, f: white.
#define COLOR_GREY_ON_BLACK 0x07

// Write a char at the cursor's current position
void arch_putc(uint8_t c);
void arch_putc_color(uint8_t c, uint8_t color);

// Clear the screen and set the cursor to the top-left position
void arch_cls(void);

#endif // __SCREEN_H__
