
#include "arch.h"
#include "lib.h"
#include "screen.h"

void rtc_tick(int none)
{
    static int ticks = 0;
    static char s = '0';
    
    if (!(++ticks % 1000))
        arch_putc(s++);
}

void int_handled(int none)
{
    arch_putc('A');
}

void kernel_main(void* mbd, unsigned int magic)
{
    if (magic != 0x2BADB002) {
        
    }

    arch_cls();
    
    arch_init();
    
    
    for (int i = 0; i < 256; i++)
        interrupt_handlers[i] = &int_handled;
        
    interrupt_handlers[32] = &rtc_tick;
    
    puts("System Initialized!\n");

    for (; ; ) { }
}
