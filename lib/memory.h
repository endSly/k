
#ifndef __MEMORY_H__
#define __MEMORY_H__

#include "types.h"

void* kmalloc(size_t size);

void* kcalloc(size_t num, size_t size);

void* krealloc(void* ptr, size_t size);

void kfree(void* ptr);

#endif // __MEMORY_H__
