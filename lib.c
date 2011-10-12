
#include "lib.h"

#include "screen.h"


int puts(const char* str)
{
    int l = 0;
    for (; str[l]; l++)
        arch_putc(str[l]);
    return l;
}

void* memset(void* ptr, int value, size_t num)
{
    uint8_t d = ptr;
    while (num--) {
        *d = value;
        d++;
    }
    return ptr;
}
