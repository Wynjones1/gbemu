#pragma once
#include <stdlib.h>
#include <stdio.h>

#define Error(M, ...) fprintf(stderr, "Error: %s:%d" M, __FILE__, __LINE__, ##__VA_ARGS__);
