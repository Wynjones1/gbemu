#include "common.h"
#include <stdarg.h>
#include <string.h>
#include <errno.h>

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
#endif
	exit(-1);
}

void common_warn(const char *format, ...)
{
#if OUTPUT_WARNINGS
	static int warn_count = 0;
	if(warn_count < WARN_LIMIT)
	{
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

void common_foutput(FILE *fp, const char *format, ...)
{
#if OUTPUT_OUTPUT
	va_list arg_list;
	va_start(arg_list, format);
	vfprintf(fp, format, arg_list);
	va_end(arg_list);
	fflush(fp);
#endif
}

void common_fread(void *ptr, size_t size, size_t nmemb, FILE *fp)
{
	int read = fread(ptr, size, nmemb, fp);
	if(read != nmemb) Error("Could not read all data.\n");
}

void common_print_binary(FILE *fp, uint64_t x, unsigned int width)
{
    for(int i = width - 1; i >= 0; i--)
    {
        fprintf(fp, "%c", ((x >> i) & 0x1) ? '1' : '0');
    }
}

FILE *common_fopen(const char *filename, const char *mode)
{
    FILE *fp = fopen(filename, mode);
    if(!fp)
    {
        Error("Could not open file %s\n", filename, strerror(errno));
    }
    return fp;
}

int common_mkdir(const char *dirname, int mode)
{
#if __MINGW32__
    return mkdir(dirname);
#else
    return mkdir(dirname, mode);
#endif
}
