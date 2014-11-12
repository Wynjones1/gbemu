#include <stdio.h>
#include <string.h>
#include "cmdline.h"

#if 0
	#if 0
		const char *rom = "/home/stuart/tetris.gb";
	#elif 0
		const char *rom = "./data/roms/mario.gb";
	#elif 0
		const char *rom = "./data/roms/zelda.gb";
	#elif 0
		const char *rom = "./data/roms/Donkey Kong Land.gb";
	#elif 0
		const char *rom = "./data/roms/Donkey Kong.gb";
	#elif 1
		const char *rom = "./data/roms/waverace.gb";
	#elif 0
		const char *rom = "./data/roms/drmario.gb";
	#elif 1
		const char *rom = "./data/roms/pokemon_blue.gb";
	#elif 0
		const char *rom = "/home/stuart/cpu_instrs.gb";
	#elif 0
		const char *rom = "/home/stuart/Documents/Gameboy/cpu_instrs/individual/01-special.gb";
	#else
		const char *rom = "/home/stuart/Documents/Gameboy/cpu_instrs/individual/05-op rp.gb";
	#endif
#endif

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
	}
	if(out.in       == NULL) out.in       = "./data/roms/mario.gb";
	if(out.boot_rom == NULL) out.boot_rom = "./data/boot_roms/DMG.bin";
	return out;
}
