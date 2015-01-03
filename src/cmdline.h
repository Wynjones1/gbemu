#pragma once
#include "common.h"

typedef struct cmdline
{
	const char *in;
    const char *boot_rom;
    const char *break_file;
    int         scale;
	bool        verbose;
    bool        audio;
    bool        replay;
    bool        record;
}cmdline_t;

/* Initialise the cmdline struct */
void cmdline_read(int argc, char **argv);
extern cmdline_t cmdline_args;
