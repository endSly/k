
#ifndef __STDARG_H__
#define __STDARG_H__

typedef char* va_list;

#define __va_size(type) ((sizeof(type)+3) & ~0x3)

#define va_start(pvar, lastarg)     \
    ((pvar) = (char*)(void*)&(lastarg) + __va_size(lastarg))

#define va_arg(pvar,type)           \
    ((pvar) += __va_size(type),     \
    *((type *)((pvar) - __va_size(type))))

#define va_end(pvar)

#endif // __STDARG_H__
