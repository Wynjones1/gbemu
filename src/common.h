#pragma once
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <signal.h>
#include <assert.h>
#include <sys/stat.h>
#include <sys/types.h>

#define WARN_LIMIT 100
#define OUTPUT_FILENAME "log.txt"

#define VERSION 1

#define Error(M, ...) common_error("Error: %s:%d in %s:\n\t" M, __FILE__, __LINE__, __func__, ##__VA_ARGS__)
#define Warning(M, ...) common_warn("Warning: %s:%d in %s:\n\t" M, __FILE__, __LINE__, __func__,##__VA_ARGS__)
#define Output(M, ...) common_output("Output: %s:%d in %s: " M, __FILE__ + 15, __LINE__, __func__,##__VA_ARGS__)
#define FOutput(fp, M, ...) common_foutput(fp, M, ##__VA_ARGS__)
#define FOPEN(filename, mode) common_fopen(filename, mode, __FILE__, __LINE__)

#if WIN32
#define NORETURN(x) __declspec(noreturn) x
#else
#define NORETURN(x) x __attribute__((noreturn))
#endif

NORETURN(void common_error(const char *format, ...));
void common_warn(const char *format, ...);
void common_output(const char *format, ...);
void common_foutput(FILE *fp, const char *format, ...);
void common_fread(void *ptr, size_t size, size_t nmemb, FILE *fp);
FILE *common_fopen(const char *filename, const char *mode, const char *func, int num);

#define BIT_N(x, n) ((x >> n) & 0x1)
#define SET_N(x, n)   x |= (1 << n)
#define RESET_N(x, n) x &= ~(1 << n)
#define ARRAYSIZE(x) (sizeof(x) / sizeof(*x))

extern uint32_t g_cycles;
