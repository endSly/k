// More info http://www.jamesmolloy.co.uk/tutorial_html/6.-Paging.html

#include "paging.h"

#include "macros.h"
#include "arch/arch.h"
#include "arch/multitask.h"
#include "arch/screen.h"
#include "lib/kprintf.h"
#include "lib/lib.h"
#include "lib/memory.h"

static uint32_t* memory_map = NULL;
static uint32_t memory_map_size = 0;
static size_t memory_size;

// Defined in linker.ld
extern int kernel_end;
uint32_t placement_address = (uint32_t) &kernel_end;
// Defined in process.s
void copy_page_physical(unsigned, unsigned);

extern uint32_t initial_esp;

page_directory* kernel_directory = NULL;
page_directory* current_directory = NULL;

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
    __asm__ volatile("mov %0, %%cr3":: "r"(dir->physical_addr));
    uint32_t cr0;
    __asm__ volatile("mov %%cr0, %0": "=r"(cr0));
    cr0 |= 0x80000000; // Enable paging!
    __asm__ volatile("mov %0, %%cr0":: "r"(cr0));
}

page_entry* get_page(uint32_t address, page_directory *dir)
{
    // Turn the address into an index.
    address /= 0x1000;
    // Find the page table containing this address.
    uint32_t table_idx = address / 1024;

    if (!dir->tables[table_idx]) {
        // this table does not exists
        dir->tables[table_idx] = (page_table*)simple_alloc(sizeof(page_table), true); // We shouldn't simple_alloc here
        dir->tables_physical[table_idx] = (uint32_t) dir->tables[table_idx] | 0x7; // PRESENT, RW, US.
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
    PANIC("No memory available");
    return 0; // Avoid warnings
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

static void free_frame(page_entry *page)
{
    uint32_t frame;
    if (frame = page->frame) {
        set_frame(frame, false);
        page->frame = 0x0;
    }
}

static void page_fault(int err_code, int int_no)
{
    // A page fault has occurred.
    // The faulting address is stored in the CR2 register.
    uint32_t faulting_address;
    __asm__ volatile("mov %%cr2, %0" : "=r" (faulting_address));

    // The error code gives us details of what happened.
    bool present = !(err_code & 0x1);   // Page not present
    bool read_only = err_code & 0x2;    // Write operation?
    bool user_mode = err_code & 0x4;    // Processor was in user-mode?
    bool reserved = err_code & 0x8;     // Overwritten CPU-reserved bits of page entry?
    bool fetch = err_code & 0x10;       // Caused by an instruction fetch?

    // Output an error message.
    kprintf("%$Page fault! (%s, %s, %s, %s, %s) at 0x%X\n", SCREEN_COLOR(RED, WHITE),
            present ? "present" : "not present",
            read_only ? "read-only" : "read-write",
            user_mode ? "user-mode" : "supervisor-mode",
            reserved ? "reserved" : "not reserved",
            fetch ? "code-fetch" : "data-fetch",
            faulting_address);

    PANIC("Page fault");
}

void* arch_alloc_pages(size_t pages_count, bool user_accesible, bool writeable)
{
    int free_page_index;
    bool free_page_found = false;

    // Find free consecutive pages to alloc
    size_t free_pages_count = 0;
    for (int table_index = 0; table_index < 1024 && !free_page_found; table_index++) {

        page_table* table = current_directory->tables[table_index];
        if (!table)
            table = (page_table*) get_page(table_index * 1024 * 0x1000, current_directory);  // table does not exists

        for (int page_index = table_index ? 0 : 1 // 0x0 is used for represent null
             ; page_index < 1024 && !free_page_found
             ; page_index++) {
            if (table->pages[page_index].frame)
                free_pages_count = 0;   // Not free page
            else
                free_pages_count++;     // free page

            free_page_index = table_index * 1024 + page_index + 1 - free_pages_count;
            free_page_found = true;
            break;
        }
    }
    if (!free_page_found)
        return NULL;

    for (int i = free_page_index; i < (free_page_index + pages_count); i++) {
        alloc_frame(&(current_directory->tables[free_page_index / 1024]->pages[free_page_index % 1024]),
                    user_accesible,
                    writeable);
    }
    return (void *) (free_page_index * 0x1000); // get the address of page
}

static page_table* clone_table(page_table *src, uint32_t *physical_addr)
{
    // Make a new page table, which is page aligned.
    page_table *table = (page_table*)krealloc(physical_addr, sizeof(page_table));
    // Ensure that the new table is blank.
    memset(table, 0, sizeof(page_table));

    for (int i = 0; i < 1024; i++) {
        if (!src->pages[i].frame)
            continue;

        // Get a new frame.
        alloc_frame(&table->pages[i], 0, 0);
        // Clone the flags from source to destination.
        if (src->pages[i].present)  table->pages[i].present = 1;
        if (src->pages[i].rw)       table->pages[i].rw = 1;
        if (src->pages[i].user_acc) table->pages[i].user_acc = 1;
        if (src->pages[i].accessed) table->pages[i].accessed = 1;
        if (src->pages[i].dirty)    table->pages[i].dirty = 1;
        // Physically copy the data across. This function is in process.s.
        copy_page_physical(src->pages[i].frame*0x1000, table->pages[i].frame*0x1000);
    }

    return table;
}

page_directory* clone_directory(page_directory *src)
{
    uint32_t phys;
    // Make a new page directory and obtain its physical address.
    page_directory *dir = (page_directory*)krealloc(&phys, sizeof(page_directory));
    // Ensure that it is blank.
    memset(dir, 0, sizeof(page_directory));
    // Get the offset of tables_physical from the start of the page_directory structure.
    uint32_t offset = (uint32_t)dir->tables_physical - (uint32_t)dir;
    // Then the physical address of dir->tables_physical is:
    dir->physical_addr = phys + offset;

    for (int i = 0; i < 1024; ++i) {
        if (!src->tables[i])
            continue;

        if (kernel_directory->tables[i] == src->tables[i]) {
            // It's in the kernel, so just use the same pointer.
            dir->tables[i] = src->tables[i];
            dir->tables_physical[i] = src->tables_physical[i];
        } else {
            // Copy the table.
            uint32_t phys;
            dir->tables[i] = clone_table(src->tables[i], &phys);
            dir->tables_physical[i] = phys | 0x07;
        }
    }

    return dir;
}

void move_stack(void *new_stack_start, uint32_t size)
{
    uint32_t i;
    // Allocate some space for the new stack.
    for(i = (uint32_t)new_stack_start;
        i >= ((uint32_t)new_stack_start-size);
        i -= 0x1000)
    {
        // General-purpose stack is in user-mode.
        alloc_frame( get_page(i, current_directory), 1, 1);
    }

    // Flush the TLB by reading and writing the page directory address again.
    uint32_t pd_addr;
    __asm__ volatile("mov %%cr3, %0" : "=r" (pd_addr));
    __asm__ volatile("mov %0, %%cr3" : : "r" (pd_addr));
    // Old ESP and EBP, read from registers.
    uint32_t old_stack_pointer;
    __asm__ volatile("mov %%esp, %0" : "=r" (old_stack_pointer));
    uint32_t old_base_pointer;
    __asm__ volatile("mov %%ebp, %0" : "=r" (old_base_pointer));

    uint32_t offset = (uint32_t)new_stack_start - initial_esp;

    uint32_t new_stack_pointer = old_stack_pointer + offset;
    uint32_t new_base_pointer  = old_base_pointer  + offset;

    // Copy the stack.
    memcpy((void*)new_stack_pointer, (void*)old_stack_pointer, initial_esp-old_stack_pointer);

    // Backtrace through the original stack, copying new values into
    // the new stack.
    for(i = (uint32_t)new_stack_start; i > (uint32_t)new_stack_start-size; i -= 4) {
        uint32_t tmp = * (uint32_t*)i;
        // If the value of tmp is inside the range of the old stack, assume it is a base pointer
        // and remap it. This will unfortunately remap ANY value in this range, whether they are
        // base pointers or not.
        if (( old_stack_pointer < tmp) && (tmp < initial_esp)) {
            tmp = tmp + offset;
            uint32_t *tmp2 = (uint32_t*)i;
            *tmp2 = tmp;
        }
    }

    // Change stacks.
    __asm__ volatile("mov %0, %%esp" : : "r" (new_stack_pointer));
    __asm__ volatile("mov %0, %%ebp" : : "r" (new_base_pointer));
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
    kernel_directory->physical_addr = (uint32_t)kernel_directory->tables_physical;

    // Declare all kernel pages
    for (int i = 0; i < placement_address; i += 0x1000) {
        alloc_frame(get_page(i, kernel_directory), true, false);
    }

    interrupt_handlers[14] = page_fault;

    // Enable paging
    switch_page_directory(kernel_directory);

    current_directory = clone_directory(kernel_directory);
    switch_page_directory(current_directory);
}
