#include "lib/kprintf.h"
#include "lib/lib.h"

#include "arch/screen.h"

// Write a string at the cursor's current position
int puts(const char* str)
{
    int l = 0;
    for (; str[l] != '\0'; l++)
        arch_putc(str[l]);
    return l;
}

void* memset(void* ptr, uint8_t value, size_t num)
{
    uint8_t* d = ptr;
    while (num--) {
        *d = value;
        ++d;
    }
    return ptr;
}

void* memcpy(void* destination, const void* source, size_t num)
{
    uint8_t* d = destination;
    const uint8_t* s = source;
    while (num--) {
        *d = *s;
        ++d;
        ++s;
    }
    return destination;
}

void panic(const char *msg)
{
    kprintf("%$Panic: %s\n", SCREEN_COLOR(RED, WHITE), msg);
}
