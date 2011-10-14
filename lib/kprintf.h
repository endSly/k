
#ifndef __KPRINTF_H__
#define __KPRINTF_H__

int kprintf(const char* format, ...);

int vkprintf(const char* format, va_list vl);

void panic(const char *format, ...);

#endif // __KPRINTF_H__
