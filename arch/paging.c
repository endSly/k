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

INLINE void set_page_usage(uint32_t page, BOOL usage)
{
    if (usage)
        memory_map[page >> 5] |= 0x1 << (page % 32);
    else
        memory_map[page >> 5] &= ~(0x1 << (page % 32));
}

INLINE size_t get_memsize(void)
{
    // Return 128 MB
    return 0x8000000;
}

INLINE void set_page_directory(page_directory* directory)
{
    current_page_directory = directory;
    __asm__ volatile("mov %0, %%cr3":: "r"(&directory->tables_physical));
    
    // Enable paging
    dword_t cr0;
    __asm__ volatile("mov %%cr0, %0": "=r"(cr0));
    cr0 |= 0x80000000;
    __asm__ volatile("mov %0, %%cr0":: "r"(cr0));
}

INLINE uint32_t get_free_page(void)
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
    
    if (page < 0) {
        // Make it swap
    }
    return (uint32_t) page;
}

INLINE void alloc_page(page_entry* page, BOOL user, BOOL writable)
{
    if (!page->address)
        return;     // Page allocated
    
    uint32_t free_page_index = get_free_page();
    set_page_usage(free_page_index, TRUE);
    
    page->presence = 1;
    page->rw = writable;
    page->user = user;
    page->address = free_page_index;
}

INLINE page_entry* get_page_entry(uint32_t page_index, page_directory* directory)
{
    uint32_t table_index = page_index / 1024;
    
    if (!directory->tables[table_index]) { // We need to alloc table
        
        uint32_t table_page = get_free_page();
        set_page_usage(table_page, TRUE);
        
        directory->tables[table_index] = (page_table*) (table_page << 12);
        directory->tables_physical[table_index] = (table_page << 12) | 0x7;
    }
    
    return &(directory->tables[table_index][page_index % 1024]);
}

static void page_fault(int none)
{
    
}

void arch_init_paging(void)
{
    memory_size = get_memsize();
    
    // Set memory map at the top of the kernel
    memory_map = (uint32_t*) &kernel_end;
    // Set new kernel top
    memory_map_size = memory_size / (PAGE_SIZE * sizeof (uint32_t)); // In words
    uint32_t kernel_top = ((uint32_t) &kernel_end) + (memory_map_size * sizeof (uint32_t));
    
    if (kernel_top & 0xFFF) // kernel_top is not aligned
        kernel_top = (kernel_top + 0x1000) & 0xFFFFF000;

    // Clear memory map
    memset(memory_map, 0, memory_map_size);
    for (int32_t kernel_page = (kernel_top >> 12) - 1
         ; kernel_page >= 0
         ; kernel_page--) {
        set_page_usage(kernel_page, TRUE);
    }

    // Set pages used by kernel
    for (int32_t kernel_page = (kernel_top >> 12) - 1
         ; kernel_page >= 0
         ; kernel_page--) {
        alloc_page(get_page_entry(kernel_page, &kernel_page_directory), 0, 0);
    }

    // Set handler
    interrupt_handlers[14] = &page_fault;
    
    // Set page directory
    //set_page_directory(&kernel_page_directory);
}


