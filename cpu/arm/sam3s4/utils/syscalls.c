/*
 *  Copyright (c) 2015, Otto-von-Guericke-Universität Magdeburg (OVGU)
 *
 *  Authors:	André Keuns		<andre.keuns@st.ovgu.de>
 *				Marcus Viererbe	<marcus.viererbe@st.ovgu.de>
 *
 *  All rights reserved
 *
 *  This script free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  (at your option) any later version.
 *
 *  The GNU General Public License can be found at
 *  http://www.gnu.org/copyleft/gpl.html.
 *
 *  This script is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  This copyright notice MUST APPEAR in all copies of the script!
 *  Thanks for all inspirations.
 *
 */

#include <stdio.h>
#include <stdarg.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <errno.h>

#undef errno
extern int errno;

extern int _close(int file);
extern void _exit(int status);
extern int _fstat(int file, struct stat *st);
extern int _getpid(void);
extern int _isatty(int file);
extern int _kill(int pid, int sig);
extern int _lseek(int file, int ptr, int dir);
extern int _open(const char *name, int flags, int mode);
extern caddr_t _sbrk(int incr);

extern int _close(int file)
{
	return -1;
}

extern void _exit(int status)
{
	printf("Exiting with status %d.\n", status);

	for (;;);
}

extern int _fstat(int file, struct stat *st)
{
	st->st_mode = S_IFCHR;
	return 0;
}

extern int _getpid(void)
{
	return 1;
}

extern int _isatty(int file)
{
	return 1;
}

extern int _kill(int pid, int sig) 
{
	errno = EINVAL;
	return -1;
}

extern int _lseek(int file, int ptr, int dir)
{
	return 0;
}

extern int _open(const char *name, int flags, int mode) {
  return -1;
}

extern caddr_t _sbrk(int incr)
{
	extern unsigned char _end;		// Defined by the linker
	extern unsigned char _ram_end_;	// Defined by the linker
	static unsigned char *heap_end;
	unsigned char *prev_heap_end;

	if (heap_end == 0) 
	{
		heap_end = &_end;
	}

	prev_heap_end = heap_end;
	
	if (heap_end + incr > &_ram_end_)
	{
		printf("Heap and stack collision\r\n");
		return (caddr_t) -1;
	}

	heap_end += incr;
	
	return (caddr_t) prev_heap_end;
}
