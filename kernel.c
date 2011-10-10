
#include "arch.h"
#include "lib.h"

void kernel_main( void* mbd, unsigned int magic )
{
    if (magic != 0x2BADB002) {
      /* Something went not according to specs. Print an error */
      /* message and halt, but do *not* rely on the multiboot */
      /* data structure. */
    }

    kcls();
    
    arch_init();
    
    kputs("System Initialized!");

    for (; ; ) { }
}
