#include "common.h"
#include <stdarg.h>
#include <signal.h>

uint32_t g_cycles = 0;
FILE *output_fp;

void common_error(const char *format, ...)
{
#if OUTPUT_ERRORS
	va_list arg_list;
	va_start(arg_list, format);
	vfprintf(stderr, format, arg_list);
	va_end(arg_list);
	if(output_fp) fflush(output_fp);
	raise(SIGABRT);
#endif

}

void common_warn(const char *format, ...)
{
#if OUTPUT_WARNINGS
	static int warn_count = 0;
	if(warn_count < WARN_LIMIT)
	{
		va_list arg_list;
		va_start(arg_list, format);
		vprintf(format, arg_list);
		va_end(arg_list);

		va_start(arg_list, format);
		vfprintf(output_fp,format, arg_list);
		va_end(arg_list);

		warn_count++;
		if(warn_count == WARN_LIMIT)
		{
			printf("Warning: Too many warnings not going to output any more.\n");
		}
	}
#endif
}

void common_output(const char *format, ...)
{
#if OUTPUT_OUTPUT
	if(!output_fp)
	{
#ifdef OUTPUT_FILENAME
		output_fp = fopen(OUTPUT_FILENAME, "w");
#else
		output_fp = stdout;
#endif

	}
	va_list arg_list;
	va_start(arg_list, format);
	vfprintf(output_fp, format, arg_list);
	va_end(arg_list);
	fflush(output_fp);
#endif
}
