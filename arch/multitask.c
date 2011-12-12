#include "arch/multitask.h"

#include "lib/lib.h"
#include "lib/memory.h"

extern uint32_t initial_esp;

void copy_page_physical(unsigned, unsigned);    // Defined in process.s

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
