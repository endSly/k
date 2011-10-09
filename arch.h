
#ifndef __ARCH_H__
#define __ARCH_H__

#include "types.h"

/*
 *  Arch specific types
 */

typedef struct {
    byte_t b[8];
} __attribute__((packed)) gdt_desc;

typedef struct {
    byte_t b[8];
} __attribute__((packed)) ldt_desc;

typedef struct idt_entry_struct
{
    word_t base_lo;
    word_t sel;
    byte_t always0;
    byte_t flags;
    word_t base_hi;
} __attribute__((packed)) idt_entry;

typedef struct {
    dword_t	link;
    dword_t	esp0, ss0;
    dword_t	esp1, ss1;
    dword_t	esp2, ss2;
    dword_t	cr3;
    dword_t	eip, eflags;
    dword_t	eax, ecx, edx, ebx, esp, ebp, esi, edi;
    dword_t	es, cs, ss, ds, fs, gs;
    dword_t	ldt;
    word_t	trace;
    word_t	iopbm_offset;
    byte_t	io_bitmap;
    byte_t	stopper;
} __attribute__((packed)) tss;

typedef struct {
    dword_t ds;
    dword_t edi, esi, ebp, esp, ebx, edx, ecx, eax;
    dword_t int_no, err_code;
    dword_t eip, cs, eflags, useresp, ss;
} __attribute__((packed)) isr_dump;


extern gdt_desc gdt[8192];
extern ldt_desc ldt[8192];
extern idt_entry idt[256];

void arch_init(void);

#endif // __ARCH_H__
