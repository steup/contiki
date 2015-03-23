#ifndef DBG_PRINTF_H_
#define DBG_PRINTF_H_

//#include <stdio.h>
//#include <string.h>

#include <debug-uart.h>
#include "strformat.h"

static StrFormatResult write_str(void *user_data, const char *data, unsigned int len);

static StrFormatContext ctxt =
	{
		write_str,
		NULL
	};

int printf(const char *fmt, ...);

#endif /* DBG_PRINTF_H_ */