
#include "arch/arch.h"
#include "arch/screen.h"
#include "lib/kprintf.h"
#include "lib/lib.h"

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
        puts("Unexpected magic!");
    }

    arch_cls();

    kprintf("\n  Welcome to k. Version: %s", KERNEL_VERSION);
    kprintf("\n--------------------------------\n"
            "Initializing System...");

    arch_init();

    //for (int i = 0; i < 256; i++) {

    //    interrupt_handlers[i] = &int_handled;
    //}

    interrupt_handlers[32] = &rtc_tick;

    kprintf("Ok!\n");

    int *ptr = (int*)0xA0000000;
    int do_page_fault = *ptr;

    for (; ; ) { }
}
