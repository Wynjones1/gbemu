#include "common.h"
#include <stdarg.h>
#include <errno.h>
#include <string.h>

uint32_t g_cycles = 0;
FILE *output_fp;

void common_error(const char *format, ...)
{
	if(OUTPUT_ERRORS)
	{
		va_list arg_list;
		va_start(arg_list, format);
		vfprintf(stderr, format, arg_list);
		va_end(arg_list);
		if(output_fp) fflush(output_fp);
	}
	exit(-1);
}

void common_warn(const char *format, ...)
{
	if(OUTPUT_WARNINGS)
	{
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
	}
}

void common_output(const char *format, ...)
{
	if(OUTPUT_OUTPUT)
	{
		if(!output_fp)
		{
			if(OUTPUT_FILENAME)
			{
				output_fp = FOPEN(OUTPUT_FILENAME, "w");
			}
			else
			{
				output_fp = stdout;
			}
		}
		va_list arg_list;
		va_start(arg_list, format);
		vfprintf(output_fp, format, arg_list);
		va_end(arg_list);
		fflush(output_fp);
	}
}

void common_foutput(FILE *fp, const char *format, ...)
{
	if(OUTPUT_OUTPUT)
	{
		va_list arg_list;
		va_start(arg_list, format);
		vfprintf(fp, format, arg_list);
		va_end(arg_list);
		fflush(fp);
	}
}

FILE *common_fopen(const char *filename, const char *mode, const char *file, int line)
{
	FILE *fp = fopen(filename, mode);
	if(!fp)
	{
		fprintf(stderr, "%s:%d Could not open file %s %s\n", file, line, filename, strerror(errno));
		exit(-1);
	}
	return fp;
}

void common_fread(void *ptr, size_t size, size_t nmemb, FILE *fp)
{
	size_t read = fread(ptr, size, nmemb, fp);
	if(read != nmemb) Error("Could not read all data.\n");
}
