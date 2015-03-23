#ifndef DBG_PUTCHAR_H_
#define DBG_PUTCHAR_H_

#include <stdio.h>
#include <debug-uart.h>
//#include <string.h>

// undefine form the stdio.h
#undef putchar
#undef putc

int putchar(int c);

int putc(int c, FILE *f);

int __sp(struct _reent *_ptr, int c, FILE *_p);

#endif /* DBG_PUTCHAR_H_ */