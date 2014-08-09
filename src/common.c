#include "common.h"
#include <stdarg.h>
#include <signal.h>

uint32_t g_cycles = 0;


void common_error(const char *format, ...)
{
#if OUTPUT_ERRORS
	va_list arg_list;
	va_start(arg_list, format);
	vprintf(format, arg_list);
	va_end(arg_list);
	raise(SIGABRT);
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

void common_output(const char *format, ...)
{
#if OUTPUT_OUTPUT
	static FILE *fp;
	if(!fp)
	{
#ifdef OUTPUT_FILENAME
		fp = fopen(OUTPUT_FILENAME, "w");
#else
		fp = stdout;
#endif

	}
	va_list arg_list;
	va_start(arg_list, format);
	vfprintf(fp, format, arg_list);
	va_end(arg_list);
	fflush(fp);
#endif
}
