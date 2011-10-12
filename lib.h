
#ifndef __LIB_H__
#define __LIB_H__

#include "types.h"

// Write a string at the cursor's current position
int puts(const char* str);

void* memset(void* ptr, int value, size_t num);
void* memcpy(void* destination, const void* source, size_t num);


#endif // __LIB_H__
