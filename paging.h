
#ifndef __PAGING_H__
#define __PAGING_H__

#include "types.h"

#define PAGE_SIZE 4096

typedef struct {
    unsigned presence:1;
    unsigned rw:1;
    unsigned user:1;
    unsigned accesed:1;
    unsigned dirty:1;
    unsigned unused:7;
    unsigned address:20;
} page_entry;

typedef page_entry page_table[1024];

// Definition of OS Page directory
typedef page_table* page_directory[1024];

void arch_init_paging(void);

void* arch_paging_alloc_pages(size_t pages_count);

#endif // __PAGING_H__
