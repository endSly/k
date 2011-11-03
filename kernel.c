
#include "arch/arch.h"
#include "arch/screen.h"
#include "lib/kprintf.h"
#include "lib/lib.h"
#include "lib/memory.h"

#define KERNEL_VERSION "0.0.1"

void rtc_tick(int none, int int_no)
{
    static int ticks = 0;
    static char s = '0';

    if (!(++ticks % 1000))
        kprintf("  Tick 0x%X!\n", ticks);
}

void int_handled(int none, int int_no)
{
    kprintf("  Interrupt 0x%X %u!\n", none, int_no);
}

void kernel_main(void* mbd, unsigned int magic)
{
    if (magic != 0x2BADB002) {
        // Something went not according to specs. Should halt
        arch_cls();
        panic("Unexpected magic!");
    }

    arch_cls();

    kprintf("\n  Welcome to k. Version: %s\n", KERNEL_VERSION);
    kprintf("--------------------------------\n"
            "Initializing System...");

    arch_init();

    //for (int i = 0; i < 256; i++) {

    //    interrupt_handlers[i] = &int_handled;
    //}

    interrupt_handlers[32] = &rtc_tick;

    kprintf("Ok!\n");
    
    // malloc test
    int* a = kmalloc(10);
    int* b = kmalloc(20);
    int* c = kmalloc(30);
    kprintf("a: 0x%X, b:0x%X, c:0x%X\n", a, b, c);
    
    kfree(b);
    int* b1 = kmalloc(5);
    int* b2 = kmalloc(5);
    int* d = kmalloc(50);
    kprintf("a: 0x%X, b1:0x%X, b2:0x%X, c:0x%X, d:0x%X\n", a, b1, b2, c, d);

    int *ptr = (int*)0xA0000000;
    int do_page_fault = *ptr;

    for (; ; ) { }
}
