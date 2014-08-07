#include "common.h"
#include <stdarg.h>

void common_error(const char *format, ...)
{
	va_list arg_list;
	va_start(arg_list, format);
	vprintf(format, arg_list);
	va_end(arg_list);
	exit(-1);
}

void common_warn(const char *format, ...)
{
	va_list arg_list;
	va_start(arg_list, format);
	vprintf(format, arg_list);
	va_end(arg_list);
}
