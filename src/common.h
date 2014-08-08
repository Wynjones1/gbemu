#pragma once
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <signal.h>

#define Error(M, ...) common_error("Error: %s:%d in %s:\n\t" M, __FILE__, __LINE__, __func__, ##__VA_ARGS__)
#define Warning(M, ...) common_warn("Warning: %s:%d in %s:\n\t" M, __FILE__, __LINE__, __func__,##__VA_ARGS__)

void common_error(const char *format, ...);
void common_warn(const char *format, ...);

#define BIT_N(x, n) ((x >> n) & 0x1)
#define SET_N(x, n)   x |= (1 << n)
#define RESET_N(x, n) x &= ~(1 << n)

extern uint32_t g_cycles;
