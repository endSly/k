
#ifndef __TASK_H__
#define __TASK_H__

typedef struct {
    dword_t link;
    dword_t esp0, ss0;
    dword_t esp1, ss1;
    dword_t esp2, ss2;
    dword_t cr3;
    dword_t eip, eflags;
    dword_t eax, ecx, edx, ebx, esp, ebp, esi, edi;
    dword_t es, cs, ss, ds, fs, gs;
    dword_t ldt;
    word_t  trace;
    word_t  iopbm_offset;
    byte_t  io_bitmap;
    byte_t  stopper;
} __attribute__((packed)) tss;

// Requested Privilege Level
#define RPL_0 (0x0)     // Supervisor
#define RPL_1 (0x1)
#define RPL_2 (0x2)
#define RPL_3 (0x3)     // User mode

#endif // __TASK_H__