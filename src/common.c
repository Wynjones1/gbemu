#include "common.h"
#include <stdarg.h>

#ifndef OUTPUT_ERRORS
	#define OUTPUT_ERRORS   0
#endif
#ifndef OUTPUT_WARNINGS
	#define OUTPUT_WARNINGS 0
#endif

void common_error(const char *format, ...)
{
#if OUTPUT_ERRORS
	va_list arg_list;
	va_start(arg_list, format);
	vprintf(format, arg_list);
	va_end(arg_list);
	//exit(-1);
	getc(stdin);
#endif
}

void common_warn(const char *format, ...)
{
#if OUTPUT_WARNINGS
	va_list arg_list;
	va_start(arg_list, format);
	vprintf(format, arg_list);
	va_end(arg_list);
#endif
}