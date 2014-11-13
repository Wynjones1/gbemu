#pragma once
#include "common.h"

typedef struct cmdline
{
	const char *in;
	const char *boot_rom;
	const char *state_file;
	bool        verbose;
}cmdline_t;

cmdline_t cmdline_read(int argc, char **argv);
