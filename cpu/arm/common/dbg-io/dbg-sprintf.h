#ifndef DBG_SPRINTF_H_
#define DBG_SPRINTF_H_

//#include <stdio.h>
#include <strformat.h>
#include <string.h>

static StrFormatResult buffer_str(void *user_data, const char *data, unsigned int len);

int sprintf(char *str, const char *format, ...);

#endif
