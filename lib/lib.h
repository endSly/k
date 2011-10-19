
#ifndef __LIB_H__
#define __LIB_H__

#include "types.h"

// Write a string at the cursor's current position
int puts(const char* str);

// Some memory functions
void* memset(void* ptr, uint8_t value, size_t num);
void* memcpy(void* destination, const void* source, size_t num);

void panic(const char *format, ...);


#endif // __LIB_H__
