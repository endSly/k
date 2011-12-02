
#ifndef __LIB_H__
#define __LIB_H__

#include "types.h"

// Some memory functions
void* memset(void* ptr, uint8_t value, size_t num);
void* memcpy(void* destination, const void* source, size_t num);

void __assert(const char* file, int line, const char* expr);

#define kassert(e) ((e) ? (void)0 : \
                    __assert(__FILE__, __LINE__, #e))

void panic(const char *msg);

#endif // __LIB_H__
