
#ifndef __KPRINTF_H__
#define __KPRINTF_H__

#include "stdarg.h"

int vkprintf(const char* format, va_list vl);

int kprintf(const char* format, ...);

#endif // __KPRINTF_H__
