#pragma once
#include <stdlib.h>
#include <stdio.h>

#define Error(M, ...) common_error("Error: %s:%d in %s:\n\t" M, __FILE__, __LINE__, __func__, ##__VA_ARGS__)
#define Warning(M, ...) common_warn("Warning: %s:%d in %s:\n\t" M, __FILE__, __LINE__, __func__,##__VA_ARGS__)

void common_error(const char *format, ...);
void common_warn(const char *format, ...);