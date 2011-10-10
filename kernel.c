
#include "arch.h"
#include "lib.h"

void rtc_tick(int none)
{
    static int ticks = 0;
    static char s = '0';
    
    if (!(++ticks % 1000))
        kputchar(s++);
}

void int_handled(int none)
{
    kputchar('A');
}

void kernel_main(void* mbd, unsigned int magic)
{
    if (magic != 0x2BADB002) {
        
    }

    kcls();
    
    arch_init();
    
    
    for (int i = 0; i < 256; i++)
        interrupt_handlers[i] = &int_handled;
        
    interrupt_handlers[32] = &rtc_tick;
    
    
    kputs("System Initialized!");

    for (; ; ) { }
}
