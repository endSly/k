#include "arch/paging.h"
#include "lib/lib.h"
#include "lib/memory.h"

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
