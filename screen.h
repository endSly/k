
#ifndef __SCREEN_H__
#define __SCREEN_H__

#include "types.h"
#include "arch.h"

void arch_putc(uint8_t c);

void arch_putc_color(uint8_t c, uint8_t color);

void arch_cls(void);

#endif // __SCREEN_H__
