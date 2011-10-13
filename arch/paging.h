
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

// Definition of Page directory
typedef struct  {
    page_table* tables[1024];
    
    dword_t tables_physical[1024];
    
} page_directory;

void arch_init_paging(void);

//void* arch_paging_alloc_page(void);

#endif // __PAGING_H__
