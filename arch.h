
#ifndef __ARCH_H__
#define __ARCH_H__

#include "types.h"

typedef struct {
    byte_t b[8];
} __attribute__((packed)) gdt_desc;

typedef struct {
    byte_t b[8];
} __attribute__((packed)) ldt_desc;

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

extern gdt_desc gdt[8192];
extern ldt_desc ldt[8192];

void arch_init(void);

void arch_set_gdt_entry(dword_t offset, dword_t base, dword_t limit, byte_t type);

#endif // __ARCH_H__
