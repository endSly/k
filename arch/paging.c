// More info http://www.jamesmolloy.co.uk/tutorial_html/6.-Paging.html

#include "paging.h"

#include "arch.h"
#include "lib.h"
#include "macros.h"
#include "lib/kprintf.h"

static uint32_t* memory_map = NULL;
static uint32_t memory_map_size = 0;
static size_t memory_size;

// Defined in linker.ld
extern int kernel_end;
uint32_t placement_address = (uint32_t) &kernel_end;

static page_directory* kernel_directory = NULL;
static page_directory* current_directory = NULL;

INLINE uint32_t simple_alloc(uint32_t sz, int align)
{
    if (align && (placement_address & 0xFFFFF000))
        placement_address = (placement_address + 0x1000) & 0xFFFFF000;

    uint32_t tmp = placement_address;
    placement_address += sz;
    return tmp;
}

INLINE void set_frame(uint32_t frame_addr, bool usage)
{
    uint32_t frame = frame_addr / 0x1000;
    // Set usage bit
    if (usage)
        memory_map[frame >> 5] |= 0x1 << (frame % 32);
    else
        memory_map[frame >> 5] &= ~(0x1 << (frame % 32));
}

INLINE size_t get_memsize(void)
{
    // Return 128 MB
    return 0x8000000;
}

INLINE void switch_page_directory(page_directory *dir)
{
    current_directory = dir;
    __asm__ volatile("mov %0, %%cr3":: "r"(&dir->tablesPhysical));
    uint32_t cr0;
    __asm__ volatile("mov %%cr0, %0": "=r"(cr0));
    cr0 |= 0x80000000; // Enable paging!
    __asm__ volatile("mov %0, %%cr0":: "r"(cr0));
}

page_entry* get_page(uint32_t address, int make, page_directory *dir)
{
    // Turn the address into an index.
    address /= 0x1000;
    // Find the page table containing this address.
    uint32_t table_idx = address / 1024;

    if (!dir->tables[table_idx]) {
        // this table is already assigned
        dir->tables[table_idx] = (page_table*)simple_alloc(sizeof(page_table), true); // We shouldn't simple_alloc here
        dir->tablesPhysical[table_idx] = (uint32_t) dir->tables[table_idx] | 0x7; // PRESENT, RW, US.
    }
    return &dir->tables[table_idx]->pages[address % 1024];
}

static uint32_t get_free_frame(void)
{
    for (uint32_t i = 0; i < memory_map_size; i++) {

        if (memory_map[i] == 0xFFFFFFFF) // nothing free, exit early.
            continue;

        // Find first free frame
        uint32_t b = 1, n = 0;
        while (memory_map[i] & b) {
            b <<= 1;
            n++;
        }

        return i * 32 + n;
    }
    return 0;
}

void alloc_frame(page_entry *page, bool user_accesible, bool writeable)
{
    if (page->frame != 0)
        return;

    uint32_t frame_index = get_free_frame();
    if (!frame_index) {
        // We have no frames availables
        // Make swap...
    }
    set_frame(frame_index * 0x1000, true);
    page->present = 1;
    page->rw = writeable;
    page->user_acc = user_accesible;
    page->frame = frame_index;
}

void free_frame(page_entry *page)
{
    uint32_t frame;
    if (frame = page->frame) {
        set_frame(frame, false);
        page->frame = 0x0;
    }
}

void arch_init_paging(void)
{
    memory_size = get_memsize();

    // Alloc memory map
    memory_map_size = memory_size / (PAGE_SIZE * sizeof(uint32_t));
    memory_map = (uint32_t*)simple_alloc(memory_map_size, false);
    memset(memory_map, 0, memory_map_size);

    // Alloc a kernel page directory.
    kernel_directory = (page_directory*)simple_alloc(sizeof(page_directory), true);
    current_directory = kernel_directory;

    // Declare all kernel pages
    for (int i = 0; i < placement_address; i += 0x1000) {
        alloc_frame(get_page(i, 1, kernel_directory), true, false);
    }

    interrupt_handlers[14] = page_fault;

    // Enable paging
    switch_page_directory(kernel_directory);
}

void page_fault(int err_code, int int_no)
{
    // A page fault has occurred.
    // The faulting address is stored in the CR2 register.
    uint32_t faulting_address;
    __asm__ volatile("mov %%cr2, %0" : "=r" (faulting_address));

    // The error code gives us details of what happened.
    int present = !(err_code & 0x1); // Page not present
    int rw = err_code & 0x2;           // Write operation?
    int us = err_code & 0x4;           // Processor was in user-mode?
    int reserved = err_code & 0x8;     // Overwritten CPU-reserved bits of page entry?
    int id = err_code & 0x10;          // Caused by an instruction fetch?

    // Output an error message.
    kprintf("Page fault! ( ");
    if (present) {kprintf("present ");}
    if (rw) {kprintf("read-only ");}
    if (us) {kprintf("user-mode ");}
    if (reserved) {kprintf("reserved ");}
    kprintf(") at 0x%X\n", faulting_address);

    panic("Page fault");
    for (; ; ) { }
}
