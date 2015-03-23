#ifndef DBG_SNPRINTF_H_
#define DBG_SNPRINTF_H_

//#include <stdio.h>
#include <strformat.h>
#include <string.h>

struct FmtBuffer
{
	char *pos;
	size_t left;
};

static StrFormatResult buffer_str(void *user_data, const char *data, unsigned int len);

int snprintf(char *str, size_t size, const char *format, ...);

int vsnprintf(char *str, size_t size, const char *format, va_list ap);

#endif /* DBG_SNPRINTF_H_ */
