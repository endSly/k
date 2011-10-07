
#include "arch.h"

// defined in loader.s
struct k_arch_gdt_entry k_arch_gdt[8192] __attribute__ ((aligned (4096)));
struct k_arch_ldt_entry k_arch_ldt[8192] __attribute__ ((aligned (4096)));

void karch_init(void)
{
    k_arch_gdt[1].b[0] = 3;
}
