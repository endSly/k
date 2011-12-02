
#ifndef __SCREEN_H__
#define __SCREEN_H__

#include "types.h"

// Colors for the screen chars
#define BLACK      0
#define BLUE       1
#define GREEN      2
#define CYAN       3
#define RED        4
#define MAGENTA    5
#define BROWN      6
#define L_GREY     7
#define D_GREY     8
#define L_BLUE     9
#define L_GREEN   10
#define L_CYAN    11
#define L_RED     12
#define L_MAGENTA 13
#define L_BROWN   14
#define WHITE     15

#define SCREEN_COLOR(bg, text) ((uint8_t)(bg << 4) | text)

// Write a char at the cursor's current position
void arch_putc_color(uint8_t c, uint8_t color);

// Clear the screen and set the cursor to the top-left position
void arch_cls(void);

#endif // __SCREEN_H__
