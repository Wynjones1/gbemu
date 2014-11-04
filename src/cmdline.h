#pragma once
#include "common.h"

typedef struct cmdline
{
	const char *in;
	int         verbose;
}cmdline_t;

cmdline_t cmdline_read(int argc, char **argv);
