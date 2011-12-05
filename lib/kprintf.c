
#include "lib/kprintf.h"

#include "arch/screen.h"
#include "lib/stdarg.h"
#include "types.h"

static void kprint_int(int32_t i, int base, bool uppercase, uint8_t color)
{
    uint32_t divisor = 1;

    if (i < 0 && base == 10 && !uppercase) {
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

void vkprintf(const char* format, va_list vl)
{
    uint8_t color = SCREEN_COLOR(BLACK, L_GREY);

    for (const char *s = format; *s; s++) {
        if (*s == '%') {
            s++;

            switch (*s) {
                case 'c': { // Character
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
                case 'd': // Signed decimal integer
                case 'i': {
                    int d = va_arg(vl, int);
                    kprint_int(d, 10, false, color);
                    break;
                }
                case 'u': { // Unsigned decimal integer
                    unsigned int d = va_arg(vl, unsigned int);
                    kprint_int(d, 10, true, color);
                    break;
                }
                case 'o': { // Unsigned octal
                    int d = va_arg(vl, int);
                    kprint_int(d, 8, false, color);
                    break;
                }
                case 'x': { // Unsigned hexadecimal integer
                    int d = va_arg(vl, int);
                    kprint_int(d, 16, false, color);
                    break;
                }
                case 'X': { // Unsigned hexadecimal integer (capital letters)
                    int d = va_arg(vl, int);
                    kprint_int(d, 16, true, color);
                    break;
                }
                case '$': // Change color
                    color = va_arg(vl, char);
                    break;
                case '%': // %
                    arch_putc_color('%', color);
                    break;
                default: // Do nothing
                    break;

            }

        } else {
            arch_putc_color(*s, color);
        }
    }
}

void kprintf(const char* format, ...)
{
    va_list ap;
    va_start(ap, format);
    vkprintf(format, ap);
    va_end(ap);
}
