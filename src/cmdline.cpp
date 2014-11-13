#include <stdio.h>
#include <string.h>
#include "cmdline.h"

#define HAVE_NEXT (i + 1 < argc)
#define NEXT argv[i + 1]
#define CMP(s, l, str)\
	(strcmp(s, str) == 0 || strcmp(l, str) == 0)
#define OPTION(s, l) CMP(s, l, argv[i])
cmdline_t cmdline_read(int argc, char **argv)
{
	cmdline_t out;
	memset(&out, 0x00, sizeof(out));
	for(int i = 0; i < argc; i++)
	{
		if(OPTION("-v", "--verbose")) out.verbose = 1;
		if(OPTION("-i", "--in"))
		{
			if(HAVE_NEXT) out.in = NEXT;
			else
				Error("Must supply input file.");
		}
		if(OPTION("-b", "--boot"))
		{
			if(HAVE_NEXT) out.boot_rom = NEXT;
			else
				Error("Must supply boot rom.");
		}
		if(OPTION("-s", "--state"))
		{
			if(HAVE_NEXT) out.state_file = NEXT;
			else
				Error("Must supply state file.");
		}
	}
	if(out.in       == NULL) out.in       = "./data/roms/mario.gb";
	if(out.boot_rom == NULL) out.boot_rom = "./data/boot_roms/DMG.bin";
	return out;
}
