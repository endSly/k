// More info http://www.jamesmolloy.co.uk/tutorial_html/6.-Paging.html

#include "paging.h"

#include "arch.h"
#include "lib.h"
#include "macros.h"

static uint32_t* memory_map = NULL;
static uint32_t memory_map_size = 0;
static size_t memory_size;

// Defined in linker.ld
extern int kernel_end;

static page_directory kernel_page_directory __attribute__ ((aligned (PAGE_SIZE)));
static page_directory* current_page_directory = NULL;

INLINE void set_page_usage(uint32_t page, int usage)
{
    if (usage)
        memory_map[page >> 5] |= 0x1 << (page % 32);
    else
        memory_map[page >> 5] &= ~(0x1 << (page % 32));
}

static size_t get_memsize(void)
{
    // Return 128 MB
    return 0x8000000;
}

static void set_page_directory(page_directory* directory)
{
    current_page_directory = directory;
    __asm__ volatile("mov %0, %%cr3":: "r"(&directory->tables_physical));
    
    // Enable paging
    dword_t cr0;
    __asm__ volatile("mov %%cr0, %0": "=r"(cr0));
    cr0 |= 0x80000000;
    __asm__ volatile("mov %0, %%cr0":: "r"(cr0));
}

// Return free page
static uint32_t swap_page(void)
{
    return 0x7000;
}

void arch_init_paging(void)
{
    memory_size = get_memsize();
    
    // Set memory map at the top of the kernel
    memory_map = (uint32_t*) &kernel_end;
    // Set new kernel top
    memory_map_size = memory_size / (PAGE_SIZE * sizeof(uint32_t)); // In words
    uint32_t kernel_top = ((uint32_t) &kernel_end) + (memory_map_size * sizeof(uint32_t));
    
    if (kernel_top & 0xFFF) // kernel_top is not aligned
        kernel_top = (kernel_top + 0x1000) & 0xFFFFF000;

    // Clear memory map
    memset(memory_map, 0, memory_map_size);
    
    // Set pages used by kernel
    for (uint32_t kernel_page = (kernel_top >> 12) - 1
         ; kernel_page >= 0
         ; kernel_page++) {
        set_page_usage(kernel_page, TRUE);
    }
}

void* arch_paging_alloc_page(void)
{
    int32_t page = -1;
    for (int i = 0; i < memory_map_size; i++) {
        if (~memory_map[i]) { // Check if all pages are in use
            uint32_t block = ~memory_map[i];
            int page_in_block = 0;
            
            while (block & 0x1) { // Find first block
                page++;
                block >>= 1;
            }
            page = i << 5 + page_in_block;
            
            break;
        }
    }
    
    if (page < 0)
        page = swap_page();
    
    return (void*) (page << 12);
}
