#pragma once
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <signal.h>

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

#define Error(M, ...) common_error("Error: %s:%d in %s:\n\t" M, __FILE__, __LINE__, __func__, ##__VA_ARGS__)
#define Warning(M, ...) common_warn("Warning: %s:%d in %s:\n\t" M, __FILE__, __LINE__, __func__,##__VA_ARGS__)
#define Output(M, ...) common_output("Output: %s:%d in %s: " M, __FILE__, __LINE__, __func__,##__VA_ARGS__)

void common_error(const char *format, ...);
void common_warn(const char *format, ...);
void common_output(const char *format, ...);

#define BIT_N(x, n) ((x >> n) & 0x1)
#define SET_N(x, n)   x |= (1 << n)
#define RESET_N(x, n) x &= ~(1 << n)

extern uint32_t g_cycles;
