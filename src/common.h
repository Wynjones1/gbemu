#pragma once
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <signal.h>
#include <stdbool.h>
#include <sys/stat.h>
#include <sys/types.h>

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

#if 1
#define OUTPUT_FILENAME "log.txt"
#endif

#ifndef DEBUG
#define DEBUG 1
#endif

#define VERSION 1

#define Error(M, ...) common_error("Error: %s:%d in %s:\n\t" M, __FILE__, __LINE__, __func__, ##__VA_ARGS__)
#define Warning(M, ...) common_warn("Warning: %s:%d in %s:\n\t" M, __FILE__, __LINE__, __func__,##__VA_ARGS__)
#define Output(M, ...) common_output("Output: %s:%d in %s: " M, __FILE__ + 15, __LINE__, __func__,##__VA_ARGS__)
#define FOutput(fp, M, ...) common_foutput(fp, M, ##__VA_ARGS__)
#define FOPEN(filename, mode) common_fopen(filename, mode)

void common_error(const char *format, ...) __attribute__((noreturn));
void common_warn(const char *format, ...);
void common_output(const char *format, ...);
void common_foutput(FILE *fp, const char *format, ...);
void common_fread(void *ptr, size_t size, size_t nmemb, FILE *fp);
FILE *common_fopen(const char *filename, const char *mode);

void common_print_binary(FILE *fp, uint64_t x, unsigned int width);

#define BIT_N(x, n) ((x >> n) & 0x1)
#define SET_N(x, n)   x |= (1 << n)
#define RESET_N(x, n) x &= ~(1 << n)
#define min(x, y) ((x) < (y) ? (x) : (y))
#define max(x, y) min(y, x)
#define PI 3.1415926

extern uint32_t g_cycles;
