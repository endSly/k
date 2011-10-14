// paging.h -- Defines the interface for and structures relating to paging.
//             Written for JamesM's kernel development tutorials.

#ifndef PAGING_H
#define PAGING_H

#define PAGE_SIZE 4096

#include "lib.h"

typedef struct {
    unsigned present  : 1;
    unsigned rw       : 1;
    unsigned user_acc : 1;   // Can be accessed from levels 1, 2 and 3
    unsigned accessed : 1;
    unsigned dirty    : 1;
    unsigned unused   : 7;
    unsigned frame    : 20;
} page_entry;

typedef struct {
    page_entry pages[1024];
} page_table;

typedef struct {
    
    page_table* tables[1024];
    
    uint32_t tablesPhysical[1024];
    
    uint32_t physicalAddr;
} page_directory;


void arch_init_paging(void);

void page_fault(int err_code, int int_no);

#endif
