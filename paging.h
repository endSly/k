
#ifndef __PAGING_H__
#define __PAGING_H__

typedef struct {
    unsigned presence:1;
    unsigned rw:1;
    unsigned user:1;
    unsigned accesed:1;
    unsigned dirty:1;
    unsigned unused:7;
    unsigned address:20;
} page_entry;

extern page_entry page_table[1024] __attribute__ ((aligned (4096)));

#endif // __PAGING_H__
