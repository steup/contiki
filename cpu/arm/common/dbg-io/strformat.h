#ifndef STRFORMAT_H_
#define STRFORMAT_H_

#include <stdarg.h>

#define STRFORMAT_OK 0
#define STRFORMAT_FAILED 1

#define HAVE_DOUBLE

#define HAVE_LONGLONG
#ifndef LARGEST_SIGNED
#ifdef HAVE_LONGLONG
#define LARGEST_SIGNED long long int
#else
#define LARGEST_UNSIGNED long int
#endif
#endif

#ifndef LARGEST_UNSIGNED
#ifdef HAVE_LONGLONG
#define LARGEST_UNSIGNED unsigned long long int
#else
#define LARGEST_UNSIGNED unsigned long int
#endif
#endif

#ifndef POINTER_INT
#define POINTER_INT unsigned long
#endif

#define MAKE_MASK(shift,size) (((1 << size) - 1) << (shift))

#define JUSTIFY_SHIFT	0
#define JUSTIFY_SIZE	1
#define JUSTIFY_RIGHT	0x0000
#define JUSTIFY_LEFT	0x0001
#define JUSTIFY_MASK	MAKE_MASK(JUSTIFY_SHIFT,JUSTIFY_SIZE)


/* How a positive number is prefixed */
#define POSITIVE_SHIFT	(JUSTIFY_SHIFT + JUSTIFY_SIZE)
#define POSITIVE_NONE	(0x0000 << POSITIVE_SHIFT)
#define POSITIVE_SPACE	(0x0001 << POSITIVE_SHIFT)
#define POSITIVE_PLUS	(0x0003 << POSITIVE_SHIFT)
#define POSITIVE_MASK	MAKE_MASK(POSITIVE_SHIFT, POSITIVE_SIZE)

#define POSITIVE_SIZE	2

#define ALTERNATE_FORM_SHIFT (POSITIVE_SHIFT + POSITIVE_SIZE)
#define ALTERNATE_FORM_SIZE 1
#define ALTERNATE_FORM  (0x0001 << ALTERNATE_FORM_SHIFT)


#define PAD_SHIFT	(ALTERNATE_FORM_SHIFT + ALTERNATE_FORM_SIZE)
#define PAD_SIZE	1
#define PAD_SPACE	(0x0000 << PAD_SHIFT)
#define PAD_ZERO	(0x0001 << PAD_SHIFT)

#define SIZE_SHIFT	(PAD_SHIFT + PAD_SIZE)
#define SIZE_SIZE	3
#define SIZE_CHAR	(0x0001 << SIZE_SHIFT)
#define SIZE_SHORT	(0x0002 << SIZE_SHIFT)
#define SIZE_INT	(0x0000 << SIZE_SHIFT)
#define SIZE_LONG	(0x0003 << SIZE_SHIFT)
#define SIZE_LONGLONG	(0x0004 << SIZE_SHIFT)
#define SIZE_MASK	MAKE_MASK(SIZE_SHIFT,SIZE_SIZE)

#define CONV_SHIFT	(SIZE_SHIFT + SIZE_SIZE)
#define CONV_SIZE	3
#define CONV_INTEGER	(0x0001 << CONV_SHIFT)
#define CONV_FLOAT	(0x0002 << CONV_SHIFT)
#define CONV_POINTER	(0x0003 << CONV_SHIFT)
#define CONV_STRING	(0x0004 << CONV_SHIFT)
#define CONV_CHAR	(0x0005 << CONV_SHIFT)
#define CONV_PERCENT	(0x0006 << CONV_SHIFT)
#define CONV_WRITTEN	(0x0007 << CONV_SHIFT)
#define CONV_MASK	MAKE_MASK(CONV_SHIFT, CONV_SIZE)

#define RADIX_SHIFT	(CONV_SHIFT + CONV_SIZE)
#define RADIX_SIZE	2
#define RADIX_DECIMAL	(0x0001 << RADIX_SHIFT)
#define RADIX_OCTAL	(0x0002 << RADIX_SHIFT)
#define RADIX_HEX	(0x0003 << RADIX_SHIFT)
#define RADIX_MASK	MAKE_MASK(RADIX_SHIFT,RADIX_SIZE)

#define SIGNED_SHIFT	(RADIX_SHIFT + RADIX_SIZE)
#define SIGNED_SIZE	1
#define SIGNED_NO	(0x0000 << SIGNED_SHIFT)
#define SIGNED_YES	(0x0001 << SIGNED_SHIFT)
#define SIGNED_MASK	MAKE_MASK(SIGNED_SHIFT,SIGNED_SIZE)

#define CAPS_SHIFT	(SIGNED_SHIFT + SIGNED_SIZE)
#define CAPS_SIZE	1
#define CAPS_NO		(0x0000 << CAPS_SHIFT)
#define CAPS_YES	(0x0001 << CAPS_SHIFT)
#define CAPS_MASK	MAKE_MASK(CAPS_SHIFT,CAPS_SIZE)

#define FLOAT_SHIFT	(CAPS_SHIFT + CAPS_SIZE)
#define FLOAT_SIZE	2
#define FLOAT_NORMAL	(0x0000 << FLOAT_SHIFT)
#define FLOAT_EXPONENT	(0x0001 << FLOAT_SHIFT)
#define FLOAT_DEPENDANT	(0x0002 << FLOAT_SHIFT)
#define FLOAT_HEX	(0x0003 << FLOAT_SHIFT)
#define FLOAT_MASK	MAKE_MASK(FLOAT_SHIFT, FLOAT_SIZE)

#define MAXCHARS_HEX ((sizeof(LARGEST_UNSIGNED) * 8) / 4 )

/* Largest number of characters needed for converting an unsigned integer.
 */
#define MAXCHARS ((sizeof(LARGEST_UNSIGNED) * 8  + 2) / 3 )

#define CHECKCB(res) {if ((res) != STRFORMAT_OK) {va_end(ap); return -1;}}

typedef unsigned int FormatFlags;

typedef unsigned int StrFormatResult;

/* The data argument may only be considered valid during the function call */
typedef StrFormatResult (*StrFormatWrite)(void *user_data, const char *data, unsigned int len);

typedef struct _StrFormatContext
{
  StrFormatWrite write_str;
  void *user_data;
} StrFormatContext;

__attribute__ ((unused))
static FormatFlags parse_flags(const char **posp )
{
	FormatFlags flags = 0;
	const char *pos = *posp;
	
	while (1)
	{
		switch(*pos)
		{
			case '-':
				flags |= JUSTIFY_LEFT;
				break;
				
			case '+':
				flags |= POSITIVE_PLUS;
				break;
				
			case ' ':
				flags |= POSITIVE_SPACE;
				break;
				
			case '#':
				flags |= ALTERNATE_FORM;
				break;
				
			case '0':
				flags |= PAD_ZERO;
				break;
				
			default:
				*posp = pos;
				return flags;
		}
		
		++pos;
	}
}

__attribute__ ((unused))
static unsigned int parse_uint(const char **posp)
{
	unsigned v = 0;
	const char *pos = *posp;
	char ch;
	
	while((ch = *pos) >= '0' && ch <= '9')
	{
		v = v * 10 + (ch - '0');
		++pos;
	}
	
	*posp = pos;
	return v;
}

__attribute__ ((unused))
static unsigned int output_uint_decimal(char **posp, LARGEST_UNSIGNED v)
{
	unsigned int len;
	char *pos = *posp;
	
	while (v > 0)
	{
		*--pos = (v % 10) + '0';
		v /= 10;
	}
	
	len = *posp - pos;
	*posp = pos;
	
	return len;
}

__attribute__ ((unused))
static unsigned int output_uint_hex(char **posp, LARGEST_UNSIGNED v, unsigned int flags)
{
	unsigned int len;
	const char *hex = (flags & CAPS_YES) ?"0123456789ABCDEF":"0123456789abcdef";
	char *pos = *posp;
	
	while (v > 0)
	{
		*--pos = hex[(v % 16)];
		v /= 16;
	}
	
	len = *posp - pos;
	*posp = pos;
	
	return len;
}

__attribute__ ((unused))
static unsigned int output_uint_octal(char **posp, LARGEST_UNSIGNED v)
{
	unsigned int len;
	char *pos = *posp;
	
	while (v > 0)
	{
		*--pos = (v % 8) + '0';
		v /= 8;
	}
	
	len = *posp - pos;
	*posp = pos;
	
	return len;
}

__attribute__ ((unused))
static StrFormatResult fill_space(const StrFormatContext *ctxt, unsigned int len)
{
	StrFormatResult res;
	static const char buffer[16] = "                ";
	
	while (len > 16)
	{
		res = ctxt->write_str(ctxt->user_data, buffer, 16);
		
		if (res != STRFORMAT_OK)
		{
			return res;
		}
		
		len -= 16;
	}
	
	return len == 0 ? STRFORMAT_OK : ctxt->write_str(ctxt->user_data, buffer, len);
}

__attribute__ ((unused))
static StrFormatResult fill_zero(const StrFormatContext *ctxt, unsigned int len)
{
	StrFormatResult res;
	static const char buffer[16] = "0000000000000000";
	
	while (len > 16)
	{
		res = ctxt->write_str(ctxt->user_data, buffer, 16);
		
		if (res != STRFORMAT_OK)
		{
			return res;
		}
		
		len -= 16;
	}

	return len == 0 ? STRFORMAT_OK : ctxt->write_str(ctxt->user_data, buffer, len);
}

int format_str(const StrFormatContext *ctxt, const char *format, ...)
	__attribute__ ((__format__ (__printf__, 2,3)));

int format_str_v(const StrFormatContext *ctxt, const char *format, va_list ap);

#endif /* STRFORMAT_H_ */
