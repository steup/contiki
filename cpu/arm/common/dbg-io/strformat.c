#include "strformat.h"

int format_str(const StrFormatContext *ctxt, const char *format, ...)
{
	int ret;
	va_list ap;
	va_start(ap, format);
	ret = format_str_v(ctxt, format, ap);
	va_end(ap);
	return ret;
}

int format_str_v(const StrFormatContext *ctxt, const char *format, va_list ap)
{
	unsigned int written = 0;
	const char *pos = format;
	
	while (*pos != '\0')
	{
		FormatFlags flags;
		unsigned int minwidth = 0;
		int precision = -1; /* Negative means no precision */
		char ch;
		const char *start = pos;
		
		while ( (ch = *pos) != '\0' && ch != '%')
		{
			++pos;
		}
		
		if (pos != start)
		{
			CHECKCB(ctxt->write_str(ctxt->user_data, start, pos - start));
			written += pos - start;
		}
		
		if (*pos == '\0')
		{
			va_end(ap);
			return written;
		}
		
		++pos;
		
		if (*pos == '\0')
		{
			va_end(ap);
			return written;
		}
		
		flags = parse_flags(&pos);

		/* parse width */
		if (*pos >= '1' && *pos <= '9')
		{
			minwidth = parse_uint(&pos);
		}
		else if (*pos == '*')
		{
			int w = va_arg(ap,int);
			
			if (w < 0)
			{
				flags |= JUSTIFY_LEFT;
				minwidth = w;
			}
			else
			{
				minwidth = w;
			}
			
			++pos;
		}

		/* parse precision */
		if (*pos == '.')
		{
			++pos;

			if (*pos >= '0' && *pos <= '9')
			{
				precision = parse_uint(&pos);
			}
			else if (*pos == '*')
			{
				++pos;
				precision = va_arg(ap,int);
			}
		}
		
		if (*pos == 'l')
		{
			++pos;
		
			if (*pos == 'l')
			{
				flags |= SIZE_LONGLONG;
				++pos;
			}
			else
			{
				flags |= SIZE_LONG;
			}
		}
		else if (*pos == 'h')
		{
			++pos;
			
			if (*pos == 'h')
			{
				flags |= SIZE_CHAR;
				++pos;
			}
			else
			{
				flags |= SIZE_SHORT;
			}
		}

		/* parse conversion specifier */
		switch(*pos)
		{
			case 'd':
			case 'i':
				flags |= CONV_INTEGER | RADIX_DECIMAL | SIGNED_YES;
				break;
				
			case 'u':
				flags |= CONV_INTEGER | RADIX_DECIMAL | SIGNED_NO;
				break;
			
			case 'o':
				flags |= CONV_INTEGER | RADIX_OCTAL | SIGNED_NO;
				break;
			
			case 'x':
				flags |= CONV_INTEGER | RADIX_HEX | SIGNED_NO;
				break;
			
			case 'X':
				flags |= CONV_INTEGER | RADIX_HEX | SIGNED_NO | CAPS_YES;
				break;
			
#ifdef HAVE_DOUBLE
			case 'f':
				flags |= CONV_FLOAT | FLOAT_NORMAL;
				break;
			
			case 'F':
				flags |= CONV_FLOAT | FLOAT_NORMAL | CAPS_YES;
				break;
			
			case 'e':
				flags |= CONV_FLOAT | FLOAT_EXPONENT;
				break;
			
			case 'E':
				flags |= CONV_FLOAT | FLOAT_EXPONENT | CAPS_YES;
				break;
			
			case 'g':
				flags |= CONV_FLOAT | FLOAT_DEPENDANT;
				break;
			
			case 'G':
				flags |= CONV_FLOAT | FLOAT_DEPENDANT | CAPS_YES;
				break;
				
			case 'a':
				flags |= CONV_FLOAT | FLOAT_HEX;
				break;
			
			case 'A':
				flags |= CONV_FLOAT | FLOAT_HEX | CAPS_YES;
				break;
			
#endif /* HAVE_DOUBLE */
			case 'c':
				flags |= CONV_CHAR;
				break;
			
			case 's':
				flags |= CONV_STRING;
				break;
			
			case 'p':
				flags |= CONV_POINTER;
				break;
			
			case 'n':
				flags |= CONV_WRITTEN;
				break;
			
			case '%':
				flags |= CONV_PERCENT;
				break;
			
			case '\0':
				va_end(ap);
				return written;
		}
		
		++pos;
		
		switch(flags & CONV_MASK)
		{
			case CONV_PERCENT:
				CHECKCB(ctxt->write_str(ctxt->user_data, "%", 1));
				++written;
				break;
				
			case CONV_INTEGER:
			{
				/* unsigned integers */
				char *prefix = 0; /* sign, "0x" or "0X" */
				unsigned int prefix_len = 0;
				char buffer[MAXCHARS];
				char *conv_pos = buffer + MAXCHARS;
				unsigned int conv_len = 0;
				unsigned int width = 0;
				unsigned int precision_fill;
				unsigned int field_fill;
				LARGEST_UNSIGNED uvalue = 0;
				int negative = 0;

				if (precision < 0)
				{
					precision = 1;
				}
				else
				{
					flags &= ~PAD_ZERO;
				}
				
				if (flags & SIGNED_YES)
				{
					/* signed integers */
					LARGEST_SIGNED value = 0;
					
					switch (flags & SIZE_MASK)
					{
						case SIZE_CHAR:
							value = (signed char)va_arg(ap, int);
							break;
							
						case SIZE_SHORT:
							value = (short)va_arg(ap, int);
							break;
						
						case SIZE_INT:
							value = va_arg(ap, int);
							break;
							
#ifdef HAVE_LONGLONG
						case SIZE_LONGLONG:
							value = va_arg(ap, long long);
							break;
#else
						case SIZE_LONGLONG:	/* Treat long long the same as long */
#endif /* HAVE_LONGLONG */

						case SIZE_LONG:
							value = va_arg(ap, long);
							break;
					}
					
					if (value < 0)
					{
						uvalue = -value;
						negative = 1;
					}
					else
					{
						uvalue = value;
					}
				}
				else
				{

					switch (flags & SIZE_MASK)
					{
						case SIZE_CHAR:
							uvalue = (unsigned char)va_arg(ap, unsigned int);
							break;
							
						case SIZE_SHORT:
							uvalue = (unsigned short)va_arg(ap, unsigned int);
							break;
							
						case SIZE_INT:
							uvalue = va_arg(ap, unsigned int);
							break;
#ifdef HAVE_LONGLONG
						case SIZE_LONGLONG:
							uvalue = va_arg(ap, unsigned long long);
							break;
#else
						case SIZE_LONGLONG:	/* Treat long long the same as long */
#endif /* HAVE_LONGLONG */

						case SIZE_LONG:
							uvalue = va_arg(ap, unsigned long);
							break;
					}
				}

				switch (flags & (RADIX_MASK))
				{
					case RADIX_DECIMAL:
						conv_len = output_uint_decimal(&conv_pos, uvalue);
						break;
						
					case RADIX_OCTAL:
						conv_len = output_uint_octal(&conv_pos, uvalue);
						break;
						
					case RADIX_HEX:
						conv_len = output_uint_hex(&conv_pos, uvalue, flags);
						break;
				}

				width += conv_len;
				
				precision_fill = (precision > conv_len) ? precision - conv_len : 0;
				
				if ((flags & (RADIX_MASK | ALTERNATE_FORM)) == (RADIX_OCTAL | ALTERNATE_FORM) && precision_fill < 1)
				{
					precision_fill = 1;
				}

				width += precision_fill;

				if ((flags & (RADIX_MASK | ALTERNATE_FORM)) == (RADIX_HEX | ALTERNATE_FORM) && uvalue != 0)
				{
					prefix_len = 2;
					
					if (flags & CAPS_YES)
					{
						prefix = "0X";
					}
					else
					{
						prefix = "0x";
					}
				}

				if (flags & SIGNED_YES)
				{
					if (negative)
					{
						prefix = "-";
						prefix_len = 1;
					}
					else
					{
						switch(flags & POSITIVE_MASK)
						{
							case POSITIVE_SPACE:
								prefix = " ";
								prefix_len = 1;
								break;
								
							case POSITIVE_PLUS:
								prefix = "+";
								prefix_len = 1;
								break;
						}
					}
				}

				width += prefix_len;

				field_fill = (minwidth > width) ? minwidth - width : 0;

				if ((flags & JUSTIFY_MASK) == JUSTIFY_RIGHT)
				{
					if (flags & PAD_ZERO)
					{
						precision_fill += field_fill;
						field_fill = 0;  /* Do not double count padding */
					}
					else
					{
						CHECKCB(fill_space(ctxt,field_fill));
					}
				}

				if (prefix_len > 0)
				{
					CHECKCB(ctxt->write_str(ctxt->user_data, prefix, prefix_len));
				}
				
				written += prefix_len;

				CHECKCB(fill_zero(ctxt,precision_fill));
				written += precision_fill;

				CHECKCB(ctxt->write_str(ctxt->user_data, conv_pos,conv_len));
				written += conv_len;

				if ((flags & JUSTIFY_MASK) == JUSTIFY_LEFT)
				{
					CHECKCB(fill_space(ctxt,field_fill));
				}
				
				written += field_fill;
			}
			break;
			
			case CONV_STRING:
			{
				unsigned int field_fill;
				unsigned int len;
				char *str = va_arg(ap,char *);
				
				if (str)
				{
					char *pos = str;
					while(*pos != '\0') pos++;
					len = pos - str;
				}
				else
				{
					str = "(null)";
					len = 6;
				}
				
				if (precision >= 0 && precision < len)
				{
					len = precision;
				}
				
				field_fill = (minwidth > len) ? minwidth - len : 0;
				
				if ((flags & JUSTIFY_MASK) == JUSTIFY_RIGHT)
				{
					CHECKCB(fill_space(ctxt,field_fill));
				}
				
				CHECKCB(ctxt->write_str(ctxt->user_data, str,len));
				written += len;
				
				if ((flags & JUSTIFY_MASK) == JUSTIFY_LEFT)
				{
					CHECKCB(fill_space(ctxt,field_fill));
				}
				
				written += field_fill;
			}
			break;
			
			case CONV_POINTER:
			{
				LARGEST_UNSIGNED uvalue = (LARGEST_UNSIGNED)(POINTER_INT)va_arg(ap,void *);
				char buffer[MAXCHARS_HEX + 3];
				char *conv_pos = buffer + MAXCHARS_HEX+3;
				unsigned int conv_len;
				unsigned int field_fill;

				conv_len = output_uint_hex(&conv_pos,uvalue,flags);
				if (conv_len == 0)
				{
					*--conv_pos = '0';
					conv_len++;
				}
				
				*--conv_pos = 'x';
				*--conv_pos = '0';
				*--conv_pos = '#';
				conv_len += 3;

				field_fill = (minwidth > conv_len) ? minwidth - conv_len : 0;

				if ((flags & JUSTIFY_MASK) == JUSTIFY_RIGHT)
				{
					CHECKCB(fill_space(ctxt,field_fill));
				}

				CHECKCB(ctxt->write_str(ctxt->user_data, conv_pos,conv_len));
				written += conv_len;

				if ((flags & JUSTIFY_MASK) == JUSTIFY_LEFT)
				{
					CHECKCB(fill_space(ctxt,field_fill));
				}
				
				written += field_fill;
			}
			break;
			
			case CONV_CHAR:
			{
				char ch = va_arg(ap,int);
				unsigned int field_fill = (minwidth > 1) ? minwidth - 1 : 0;
				
				if ((flags & JUSTIFY_MASK) == JUSTIFY_RIGHT)
				{
					CHECKCB(fill_space(ctxt,field_fill));
					written += field_fill;
				}

				CHECKCB(ctxt->write_str(ctxt->user_data, &ch, 1));
				written++;

				if ((flags & JUSTIFY_MASK) == JUSTIFY_LEFT)
				{
					CHECKCB(fill_space(ctxt,field_fill));
				}
				
				written+= field_fill;
			}
			break;
			
			case CONV_WRITTEN:
			{
				int *p = va_arg(ap,int*);
				*p = written;
			}
			break;
		}
	}

	return written;
}
