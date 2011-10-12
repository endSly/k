#include "paging.h"

page_directory kernel_page_directory __attribute__ ((aligned (PAGE_SIZE)));

page_directory* current_page_directory = NULL;

void arch_set_page_directory(page_directory* directory)
{
    current_page_directory = directory;
}

void arch_init_paging(void)
{
    
}

void* arch_paging_alloc_page(void)
{
    return NULL;
}
