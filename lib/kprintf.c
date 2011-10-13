
#include "kprintf.h"

#include "stdarg.h"
#include "types.h"
#include "arch/screen.h"

static void kprint_int(int32_t i, int base, BOOL uppercase)
{
    int32_t divisor = 1;
    
    if (i < 0) {
        arch_putc('-');
        i = -i;
    }
    
    while ((i / divisor) >= base) {
        divisor *= base;
    }
        
    do {
        int char_value = i / divisor;
        
        if (char_value < 10) {
            arch_putc('0' + char_value);
        } else if (uppercase) {
            arch_putc('A' + char_value - 10);
        } else {
            arch_putc('a' + char_value - 10);
        }
        
        i = i % divisor;
        divisor /= base;
        
    } while (divisor >= 1);
}

static void kprint_float(double f)
{
    
}

static int vkprintf(const char* format, va_list vl) 
{
    int chars = 0;
    for (const char* s = format; *s; s++) {
        if (*s == '%') {
            s++;
            
            if (*s == 'l')
                s++;    // Long types are not supported!
            
            switch (*s++) {
                case 'c': { // Char
                    char c = va_arg(vl, char);
                    arch_putc(c);
                    break;
                }
                case 's': { // String
                    const char* str = va_arg(vl, const char*);
                    for (; *str; str++)
                        arch_putc(*str);
                    break;
                }
                case 'd': { // Decimal
                    int d = va_arg(vl, int);
                    kprint_int(d, 10, FALSE);
                    break;
                }
                case 'o': { // Octal
                    int d = va_arg(vl, int);
                    kprint_int(d, 8, FALSE);
                    break;
                }
                case 'x': { // Hex
                    int d = va_arg(vl, int);
                    kprint_int(d, 16, FALSE);
                    break;
                }
                case 'X': { // Hex uppercase
                    int d = va_arg(vl, int);
                    kprint_int(d, 16, TRUE);
                    break;
                }
                case 'u': { // Unsigned
                    unsigned int d = va_arg(vl, unsigned int);
                    kprint_int(d, 10, TRUE);
                    break;
                }
                default: // Do nothing
                    break;
                    
            }
                
        }
        chars++;
        arch_putc(*s);
    }
    return chars;
}


int kprintf(const char* format, ...)
{
    va_list ap;
    va_start(ap, format);
    int result = vkprintf(format, ap);
    va_end(ap);
    
    return result;
}
