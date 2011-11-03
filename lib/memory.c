
#include "memory.h"

#include "lib/lib.h"
#include "arch/paging.h"

/*  Simple alloc algorithm
 *  
 *  [                            Page                            ]
 * - Before malloc
 *  |mem_block|____data____|mem_block|___________free____________|
 *
 * - After malloc
 *  |mem_block|____data____|mem_block|__data__|mem_block|__free__|
 *
 * Additionally memory holes are ordered by it's size
 *
 */

typedef struct mem_block {
    size_t size;
    struct mem_block* next; // Ordered list of free blocks (size desc)
    unsigned used  : 1;     // Is a Hole?
    unsigned magic : 31;    // Magic for error checking
} mem_block;

#define MAGIC (0x7AAA5555)

// Ordered list of free blocks (size desc)
static mem_block* free_blocks_list = NULL;

void* kmalloc(size_t size)
{
    if (!free_blocks_list || free_blocks_list->size < size) {
        // We have no hole for alloc this space. Alloc enough pages for size and it's block definition
        size_t pages_count = 1 + (size + sizeof(mem_block) - 1) / PAGE_SIZE;
        void* alloc_space = arch_alloc_pages(pages_count, true, true);
        
        mem_block* new_block = (mem_block*) alloc_space;
        
        new_block->size = pages_count * PAGE_SIZE - sizeof(mem_block);
        new_block->next = free_blocks_list;
        new_block->used = false; // Not used by now
        new_block->magic = MAGIC;
        
        // new_block is biggest
        free_blocks_list = new_block;
    }
    
    // Find smallest block to fit it
    mem_block* prev_smallest_block = NULL;
    mem_block* smallest_block = free_blocks_list;
    while (smallest_block->size > size 
           && smallest_block->next 
           && smallest_block->next->size >= size) {
        prev_smallest_block = smallest_block;
        smallest_block = smallest_block->next;
    }
    
    kassert(smallest_block && smallest_block->size >= (size + sizeof(mem_block)));
    kassert(smallest_block->magic == MAGIC);
    smallest_block->used = true;
    
    if (prev_smallest_block)
        prev_smallest_block->next = smallest_block->next;
    else
        free_blocks_list = smallest_block->next;
    
    if (smallest_block->size > (size + sizeof(mem_block))) {
        // We have space for split it
        memblock* new_block = ((void*) smallest_block) + size + sizeof(mem_block);
        new_block->size = smallest_block->size - size - sizeof(mem_block);
        new_block->used = false;
        new_block->magic = MAGIC;
        
        smallest_block->size = size;
        
        // Insert new block
        memblock* prev_block = NULL;
        memblock* block = free_blocks_list;
        while (block && block->size > new_block->size) {
            prev_block = block;
            block = block->next;
        }
        prev_block->next = new_block;
        new_block->next = block;
    }
    
    return ((void*) smallest_block) + sizeof(mem_block);
}

void* kcalloc(size_t num, size_t size)
{
    size_t length = num * size;
    void* ptr = kmalloc(length);
    memset(ptr, 0, length);
    return ptr;
}

void* krealloc(void* ptr, size_t size)
{
    void* new_ptr = NULL;
    if (ptr) {
        mem_block* block = (mem_block*) (ptr - sizeof(mem_block));
        if (block->size < size) {
            
        } else if (block->size > size) {
            
        } else {
            new_ptr = ptr;
        }
    }
    return new_ptr;
}

void kfree(void* ptr)
{
    if (ptr) {
        // Free block
        mem_block* block = (mem_block*) (ptr - sizeof(mem_block));
        block->used = false;
        
        ptr += block->size;
        mem_block* next_block = (mem_block*) ptr;
    }
}
