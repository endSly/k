
#ifndef __TYPES_H__
#define __TYPES_H__

/*
 *  Architecture types
 */
typedef unsigned long long  qword_t;
typedef unsigned int        dword_t;
typedef unsigned short      word_t;
typedef unsigned char       byte_t;

typedef signed long long    sqword_t;
typedef signed int          sdword_t;
typedef signed short        sword_t;
typedef signed char         sbyte_t;

typedef dword_t*            ptr_t;

/*
 *  Fixed Size Types
 */
typedef unsigned long long  uint64_t;
typedef unsigned long       uint32_t;
typedef unsigned short      uint16_t;
typedef unsigned char       uint8_t;
typedef unsigned int        uint_t;
typedef long long           int64_t;
typedef long                int32_t;
typedef short               int16_t;
typedef char                int8_t;

typedef unsigned int        size_t;

#ifndef NULL
#define NULL ((void*)0)
#endif

// c99's  stdbool.h

typedef char bool;

#ifndef false
#define false (0)
#endif

#ifndef true
#define true (1)
#endif

#ifndef __bool_true_false_are_defined
#define __bool_true_false_are_defined 1
#endif

#endif // __TYPES_H__
