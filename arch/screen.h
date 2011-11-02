
#ifndef __SCREEN_H__
#define __SCREEN_H__

#include "macros.h"
#include "types.h"

// Colors for the screen chars
#define COLOR_GREY_ON_BLACK 0x07

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
INLINE void arch_putc(uint8_t c) {
    arch_putc_color(c, COLOR_GREY_ON_BLACK);
}

// Clear the screen and set the cursor to the top-left position
void arch_cls(void);

#endif // __SCREEN_H__
