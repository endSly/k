
#include "arch.h"

// defined in loader.s
struct k_arch_gdt_entry k_arch_gdt[8192] __attribute__ ((aligned (4)));
struct k_arch_ldt_entry k_arch_ldt[8192] __attribute__ ((aligned (4)));

struct k_arch_tss tss __attribute__ ((aligned (4)));

void karch_init(void)
{
    karch_set_gdt_entry(0, 0, 0, 0); // Set null selector
    karch_set_gdt_entry(1, 0, 0xffffffff, 0x9A); // Code
    karch_set_gdt_entry(2, 0, 0xffffffff, 0x9A); // Data
    karch_set_gdt_entry(3, &tss, sizeof(struct k_arch_tss), 0x9A); // TSS
}


void karch_set_gdt_entry(unsigned int index, unsigned int base, unsigned int limit, unsigned char type)
{
    struct k_arch_gdt_entry *entry = &k_arch_gdt[index];
    
    if (limit > 0xFFFF) { // Adjust granularity if required
        limit = limit >> 12;
        entry->b[6] = 0xC0;
    } else {
        entry->b[6] = 0x40;
    }
    
    // limit
    entry->b[0] = limit & 0xFF;
    entry->b[1] = (limit >> 8) & 0xFF;
    entry->b[6] |= (limit >> 16) & 0xF;
    
    // base 
    entry->b[2] = base & 0xFF;
    entry->b[3] = (base >> 8) & 0xFF;
    entry->b[4] = (base >> 16) & 0xFF;
    entry->b[7] = (base >> 24) & 0xFF;
    
    // type
    entry->b[5] = type;
}
