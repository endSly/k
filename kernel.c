
#include "arch.h"

void kernel_main( void* mbd, unsigned int magic )
{
    if (magic != 0x2BADB002) {
      /* Something went not according to specs. Print an error */
      /* message and halt, but do *not* rely on the multiboot */
      /* data structure. */
    }

    char* boot_loader_name =(char*) ((long*)mbd)[16];
 
    unsigned char* screen = (unsigned char *) 0xb8000;
    
    for (int i = 0; i < 80; i++) {
        screen[i << 1] = boot_loader_name[i]; 
        screen[(i << 1) + 1] = 0x07;
    }
    
    arch_init();
    
    for (; ; ) { }
}
