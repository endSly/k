
#include "paging.h"

#include "arch.h"

static dword_t* memory_map;
static size_t memory_size;

// Defined in linker.ld
extern int kernel_end;

static page_directory kernel_page_directory __attribute__ ((aligned (PAGE_SIZE)));
static page_directory* current_page_directory = NULL;

static size_t arch_get_memsize(void)
{
    // Return 128 MB
    return 0x8000000;
}

static void arch_set_page_directory(page_directory* directory)
{
    current_page_directory = directory;
}

void arch_init_paging(void)
{
    memory_size = arch_get_memsize();
    memory_map = arch_paging_alloc_pages(memory_size / (sizeof(dword_t) * PAGE_SIZE));

}

void* arch_paging_alloc_pages(size_t pages_count)
{
    return NULL;
}
