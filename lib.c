
#include "lib.h"

#include "screen.h"


int puts(const char* str)
{
    int l = 0;
    for (; str[l]; l++)
        arch_putc(str[l]);
    return l;
}
