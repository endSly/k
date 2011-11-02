
#include "memory.h"

#include "lib.h"
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
 */
 

typedef stuct {
    size_t size;
    bool used;
} mem_block;

void* kmalloc(size_t size)
{
    
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
