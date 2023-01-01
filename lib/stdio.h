#ifndef LIB_STDIO_H_
#define LIB_STDIO_H_

#include "stdarg.h"
#define NULL ((void *)0)

typedef enum utoa_t
{
    utoa_dec = 10,
    utoa_hex = 16,
} utoa_t;

uint32_t putstr(const char* s);
uint32_t debug_printf(const char* format, ...);
#endif /* LIB_STDIO_H_*/