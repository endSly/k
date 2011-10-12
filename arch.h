
#ifndef __ARCH_H__
#define __ARCH_H__

#include "types.h"
#include "macros.h"
#include "paging.h"

typedef void (*interrupt_handler)(int err_code);

extern interrupt_handler interrupt_handlers[256];

INLINE void outb(word_t port, word_t value) {
    __asm__ volatile ("outb %1, %0" : : "dN" (port), "a" (value));
}

INLINE byte_t inb(word_t port) {
    byte_t ret;
    __asm__ volatile("inb %1, %0" : "=a" (ret) : "dN" (port));
    return ret;
}

INLINE word_t inw(word_t port) {
    word_t ret;
    __asm__ volatile ("inw %1, %0" : "=a" (ret) : "dN" (port));
    return ret;
}

void arch_init(void);

#endif // __ARCH_H__
