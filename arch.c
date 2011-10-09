
#include "macros.h"

#include "arch.h"

void arch_init_gdt(void);
void arch_set_gdt_desc(dword_t offset, dword_t base, dword_t limit, byte_t type);
void arch_init_idt(void);
void arch_set_idt_entry(int index, dword_t base, word_t sel, byte_t flags);

void gdt_flush(dword_t gdt_ptr);
void idt_flush(dword_t gdt_ptr);

gdt_desc gdt[8192] __attribute__ ((aligned (4))); // __attribute__ ((aligned (4096))); if pagination enabled
ldt_desc ldt[8192] __attribute__ ((aligned (4))); // __attribute__ ((aligned (4096))); if pagination enabled
idt_entry idt[256] __attribute__ ((aligned (4)));

tss os_tss __attribute__ ((aligned (4)));

INLINE void outb(word_t port, word_t value)
{
    __asm__ volatile ("outb %1, %0" : : "dN" (port), "a" (value));
}

INLINE byte_t inb(word_t port)
{
    byte_t ret;
    __asm__ volatile("inb %1, %0" : "=a" (ret) : "dN" (port));
    return ret;
}

INLINE word_t inw(word_t port)
{
    word_t ret;
    __asm__ volatile ("inw %1, %0" : "=a" (ret) : "dN" (port));
    return ret;
}

struct table_ptr { word_t limit; dword_t base; } __attribute__((packed));

void arch_init(void)
{
    arch_init_gdt();
    arch_init_idt();
}

/*
 *  GDT Functions
 */

void arch_init_gdt(void)
{
    // Setup GDT
    arch_set_gdt_desc(0x00, 0, 0, 0); // Set null selector
    arch_set_gdt_desc(0x08, 0, 0xffffffff, 0x9A); // Code
    arch_set_gdt_desc(0x10, 0, 0xffffffff, 0x92); // Data
    arch_set_gdt_desc(0x18, (dword_t) &os_tss, sizeof(tss), 0x89); // TSS
    
    // Load GDT
    struct table_ptr gdt_desc = {sizeof(gdt), (dword_t) gdt} ;
    gdt_flush((dword_t) &gdt_desc);
}


void arch_set_gdt_desc(dword_t offset, dword_t base, dword_t limit, byte_t type)
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

/*
 *  IDT Functions
 */

// Definition of isr functions
void isr0(void);
void isr1(void);
void isr2(void);
void isr3(void);
void isr4(void);
void isr5(void);
void isr6(void);
void isr7(void);
void isr8(void);
void isr9(void);
void isr10(void);
void isr11(void);
void isr12(void);
void isr13(void);
void isr14(void);
void isr15(void);
void isr16(void);
void isr17(void);
void isr18(void);
void isr19(void);
void isr20(void);
void isr21(void);
void isr22(void);
void isr23(void);
void isr24(void);
void isr25(void);
void isr26(void);
void isr27(void);
void isr28(void);
void isr29(void);
void isr30(void);
void isr31(void);

void arch_init_idt(void)
{
    
    arch_set_idt_entry(0, (dword_t)&isr0, 0x08, 0x8E);
    arch_set_idt_entry(1, (dword_t)&isr1, 0x08, 0x8E);
    arch_set_idt_entry(2, (dword_t)&isr2, 0x08, 0x8E);
    arch_set_idt_entry(3, (dword_t)&isr3, 0x08, 0x8E);
    arch_set_idt_entry(4, (dword_t)&isr4, 0x08, 0x8E);
    arch_set_idt_entry(5, (dword_t)&isr5, 0x08, 0x8E);
    arch_set_idt_entry(6, (dword_t)&isr6, 0x08, 0x8E);
    arch_set_idt_entry(7, (dword_t)&isr7, 0x08, 0x8E);
    arch_set_idt_entry(8, (dword_t)&isr8, 0x08, 0x8E);
    arch_set_idt_entry(9, (dword_t)&isr9, 0x08, 0x8E);
    arch_set_idt_entry(10, (dword_t)&isr10, 0x08, 0x8E);
    arch_set_idt_entry(11, (dword_t)&isr11, 0x08, 0x8E);
    arch_set_idt_entry(12, (dword_t)&isr12, 0x08, 0x8E);
    arch_set_idt_entry(13, (dword_t)&isr13, 0x08, 0x8E);
    arch_set_idt_entry(14, (dword_t)&isr14, 0x08, 0x8E);
    arch_set_idt_entry(15, (dword_t)&isr15, 0x08, 0x8E);
    arch_set_idt_entry(16, (dword_t)&isr16, 0x08, 0x8E);
    arch_set_idt_entry(17, (dword_t)&isr17, 0x08, 0x8E);
    arch_set_idt_entry(18, (dword_t)&isr18, 0x08, 0x8E);
    arch_set_idt_entry(19, (dword_t)&isr19, 0x08, 0x8E);
    arch_set_idt_entry(20, (dword_t)&isr20, 0x08, 0x8E);
    arch_set_idt_entry(21, (dword_t)&isr21, 0x08, 0x8E);
    arch_set_idt_entry(22, (dword_t)&isr22, 0x08, 0x8E);
    arch_set_idt_entry(23, (dword_t)&isr23, 0x08, 0x8E);
    arch_set_idt_entry(24, (dword_t)&isr24, 0x08, 0x8E);
    arch_set_idt_entry(25, (dword_t)&isr25, 0x08, 0x8E);
    arch_set_idt_entry(26, (dword_t)&isr26, 0x08, 0x8E);
    arch_set_idt_entry(27, (dword_t)&isr27, 0x08, 0x8E);
    arch_set_idt_entry(28, (dword_t)&isr28, 0x08, 0x8E);
    arch_set_idt_entry(29, (dword_t)&isr29, 0x08, 0x8E);
    arch_set_idt_entry(30, (dword_t)&isr30, 0x08, 0x8E);
    arch_set_idt_entry(31, (dword_t)&isr31, 0x08, 0x8E);

    // Load IDT
    struct table_ptr idt_desc = {sizeof(idt), (dword_t)idt} ;
    idt_flush((dword_t) &idt_desc);
}

void arch_set_idt_entry(int index, dword_t base, word_t sel, byte_t flags)
{
    idt[index].base_lo = base & 0xFFFF;
    idt[index].base_hi = (base >> 16) & 0xFFFF;
    
    idt[index].sel = sel;
    idt[index].always0 = 0;
    // We must uncomment the OR below when we get to using user-mode.
    // It sets the interrupt gate's privilege level to 3.
    idt[index].flags   = flags /* | 0x60 */;
}

void isr_handler(const isr_dump dump) 
{
    
}
