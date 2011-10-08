
#include "arch.h"

gdt_desc gdt[8192] __attribute__ ((aligned (4)));
ldt_desc ldt[8192] __attribute__ ((aligned (4)));

tss os_tss __attribute__ ((aligned (4)));

void arch_init(void)
{
    arch_set_gdt_entry(0, 0, 0, 0); // Set null selector
    arch_set_gdt_entry(1, 0, 0xffffffff, 0x9A); // Code
    arch_set_gdt_entry(2, 0, 0xffffffff, 0x9A); // Data
    arch_set_gdt_entry(3, (dword_t) &os_tss, sizeof(tss), 0x9A); // TSS
}


void arch_set_gdt_entry(dword_t offset, dword_t base, dword_t limit, byte_t type)
{
    gdt_desc *desc = gdt + offset;
    
    if (limit > 0xFFFF) { // Adjust granularity if required
        limit = limit >> 12;
        desc->b[6] = 0xC0;
    } else {
        desc->b[6] = 0x40;
    }
    
    // limit
    desc->b[0] = limit & 0xFF;
    desc->b[1] = (limit >> 8) & 0xFF;
    desc->b[6] |= (limit >> 16) & 0xF;
    
    // base 
    desc->b[2] = base & 0xFF;
    desc->b[3] = (base >> 8) & 0xFF;
    desc->b[4] = (base >> 16) & 0xFF;
    desc->b[7] = (base >> 24) & 0xFF;
    
    // type
    desc->b[5] = type;
}
