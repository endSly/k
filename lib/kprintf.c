
#include "lib/kprintf.h"

#include "arch/screen.h"
#include "lib/stdarg.h"
#include "types.h"

static void kprint_int(int32_t i, int base, bool uppercase, uint8_t color)
{
    uint32_t divisor = 1;

    if (i < 0 && base == 10) {
        arch_putc_color('-', color);
        i = -i;
    }

    while ((i / divisor) >= base) {
        divisor *= base;
    }

    do {
        int char_value = i / divisor;

        if (char_value < 10) {
            arch_putc_color('0' + char_value, color);
        } else {
            arch_putc_color(uppercase ? 'A' : 'a' + char_value - 10, color);
        }

        i = i % divisor;
        divisor /= base;

    } while (divisor >= 1);
}

static void kprint_float(double f)
{

}

void vkprintf(const char* format, va_list vl)
{
    uint8_t color = SCREEN_COLOR(BLACK, L_GREY);

    for (const char *s = format; *s; s++) {
        if (*s == '%') {
            s++;

            switch (*s++) {
                case 'c': { // Char
                    char c = va_arg(vl, char);
                    arch_putc_color(c, color);
                    break;
                }
                case 's': { // String
                    const char* str = va_arg(vl, const char*);
                    for (; *str; ++str)
                        arch_putc_color(*str, color);
                    break;
                }
                case 'd': { // Decimal
                    int d = va_arg(vl, int);
                    kprint_int(d, 10, false, color);
                    break;
                }
                case 'l': { // Long
                    // Long types are not supported!
                    break;
                }
                case 'o': { // Octal
                    int d = va_arg(vl, int);
                    kprint_int(d, 8, false, color);
                    break;
                }
                case 'x': { // Hex
                    int d = va_arg(vl, int);
                    kprint_int(d, 16, false, color);
                    break;
                }
                case 'X': { // Hex uppercase
                    int d = va_arg(vl, int);
                    kprint_int(d, 16, true, color);
                    break;
                }
                case 'u': { // Unsigned
                    unsigned int d = va_arg(vl, unsigned int);
                    kprint_int(d, 10, true, color);
                    break;
                }
                case '$': { // Change color
                    color = va_arg(vl, char);
                    break;
                }
                default: // Do nothing
                    break;

            }

        }
        arch_putc_color(*s, color);
    }
}

void kprintf(const char* format, ...)
{
    va_list ap;
    va_start(ap, format);
    vkprintf(format, ap);
    va_end(ap);
}
