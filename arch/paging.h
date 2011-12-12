
#ifndef __PAGING_H__
#define __PAGING_H__

#include "types.h"

#define PAGE_SIZE 4096

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
    // Pointers To tables
    page_table* tables[1024];

    // Tables physical address
    uint32_t tables_physical[1024];

    // tables_physical physical address
    uint32_t physical_addr;
} page_directory;

extern page_directory* kernel_directory;
extern page_directory* current_directory;

void arch_init_paging(void);

page_entry* get_page(uint32_t address, page_directory *dir);

void alloc_frame(page_entry *page, bool user_accesible, bool writeable);
void* arch_alloc_pages(size_t pages_count, bool user_accesible, bool writeable);

#endif // __PAGING_H__
