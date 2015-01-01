#pragma once
#include "common.h"

typedef struct cmdline
{
	const char *in;
    const char *boot_rom;
    int         scale;
	bool        verbose;
    bool        audio;
    bool        replay;
    bool        record;
}cmdline_t;

cmdline_t cmdline_read(int argc, char **argv);
