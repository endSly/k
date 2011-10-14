
#ifndef __ARCH_H__
#define __ARCH_H__

#include "types.h"
#include "macros.h"
#include "paging.h"

typedef void (*interrupt_handler)(int err_code, int int_no);

extern interrupt_handler interrupt_handlers[256];

// Write a byte out to the specified port.
INLINE void outb(word_t port, byte_t value)
{
    __asm__ volatile ("outb %1, %0" : : "dN" (port), "a" (value));
}

// Read a byte from a port
INLINE byte_t inb(word_t port)
{
    byte_t ret;
    __asm__ volatile("inb %1, %0" : "=a" (ret) : "dN" (port));
    return ret;
}

// Read a word from a port
INLINE word_t inw(word_t port)
{
    word_t ret;
    __asm__ volatile ("inw %1, %0" : "=a" (ret) : "dN" (port));
    return ret;
}

void arch_init(void);

#endif // __ARCH_H__
