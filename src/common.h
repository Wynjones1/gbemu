#pragma once
#include <stdlib.h>
#include <stddef.h>
#include <stdio.h>
#include <stdint.h>
#include <signal.h>
#include <stddef.h>
#include <stdbool.h>
#include <sys/stat.h>
#include <sys/types.h>

#include "SDL.h"
#if HAVE_TTF
    #include "SDL_ttf.h"
#else
    typedef void *TTF_Font;
#endif

#ifndef OUTPUT_ERRORS
	#define OUTPUT_ERRORS   1
#endif
#ifndef OUTPUT_WARNINGS
	#define OUTPUT_WARNINGS 1
#endif

#ifndef OUTPUT_OUTPUT
	#define OUTPUT_OUTPUT 1
#endif

#ifndef WARN_LIMIT
	#define WARN_LIMIT 100
#endif

#ifndef TESTING
    #define TESTING 0
#endif

#if TESTING
    #define CPU_DUMMY_IO 1
#else
    #define CPU_DUMMY_IO 0
#endif

#ifndef OUTPUT_DEBUG_FILES
    #define OUTPUT_DEBUG_FILES 0
#endif

#ifndef AUDIO
    #define AUDIO 1
#endif

#ifndef SPINLOCK
    #define SPINLOCK 0
#endif

#ifndef SORT_OAM
    #define SORT_OAM 0
#endif

#ifndef DEBUG_WINDOW
    #define DEBUG_WINDOW 0
#endif


#if 1
#define OUTPUT_FILENAME "log.txt"
#endif

#ifndef DEBUG
#define DEBUG 0
#endif

#if DEBUG || 1
#define SDL_Error(cond)                       \
    do{if(cond) Error("%s\n", SDL_GetError());}while(0)
#define TTF_Error(cond)                       \
    do{if(cond) Error("%s\n", TTF_GetError());}while(0)
#else
#define SDL_Error(cond)do{int i = (cond);}while(0)
#define TTF_Error(cond)do{int i = (cond);}while(0)
#endif

#define VERSION 1

#if __WIN32__
#define NORETURN(function) function
#define CURRENT_FUNC "placeholder_function"
#else
#define NORETURN(function) function __attribute__((noreturn))
#define CURRENT_FUNC __func__
#endif

#define Error(M, ...) common_error("Error: %s:%d in %s:\n\t" M, __FILE__, __LINE__, CURRENT_FUNC, ##__VA_ARGS__)
#define Warning(M, ...) common_warn("Warning: %s:%d in %s:\n\t" M, __FILE__, __LINE__, CURRENT_FUNC,##__VA_ARGS__)
#define Output(M, ...) common_output("Output: %s:%d in %s: " M, __FILE__ + 15, __LINE__, CURRENT_FUNC,##__VA_ARGS__)
#define FOutput(fp, M, ...) common_foutput(fp, M, ##__VA_ARGS__)
#define FOPEN(filename, mode) common_fopen(filename, mode)



int common_mkdir(const char *dirname, int mode);
NORETURN(void common_error(const char *format, ...));
void common_warn(const char *format, ...);
void common_output(const char *format, ...);
void common_foutput(FILE *fp, const char *format, ...);
void common_fread(void *ptr, size_t size, size_t nmemb, FILE *fp);
FILE *common_fopen(const char *filename, const char *mode);

void common_print_binary(FILE *fp, uint64_t x, unsigned int width);

#define BIT_N(x, n) ((x >> n) & 0x1)
#define SET_N(x, n)   (x |= (1 << n))
#define RESET_N(x, n) (x &= ~(1 << n))
#ifndef min
	#define min(x, y) ((x) < (y) ? (x) : (y))
#endif
#ifndef max
	#define max(x, y) ((x) > (y) ? (x) : (y))
#endif
#define PI 3.1415926

#define MASK(width) ((1 << (width)) - 1)
#define ENCODE(x, msb, lsb) (((x) & MASK(msb + 1 - lsb)) << lsb)
#define DECODE(x, msb, lsb) (((x) & MASK(msb + 1))       >> lsb)

void *common_malloc(size_t num);
void *common_calloc(size_t count, size_t size);
void *common_realloc(void *block, size_t size);

#define MALLOC(num_bytes)    common_malloc(num_bytes)
#define CALLOC(num, size)    common_calloc(num, size)
#define REALLOC(block, size) common_realloc(block, size)

extern uint32_t g_cycles;
