#include <stdio.h>
#include <string.h>

#include "rom.h"
#include "cpu.h"
#include "display.h"
#include "ppm.h"
#include "testing/testing.h"

#include <SDL2/SDL.h>
#define TESTING 0

int main(int argc, char **argv)
{
	#if TESTING
		testing(argc, argv);
	#else
		#if 0
			const char *rom = "/home/stuart/tetris.gb";
		#elif 1
			const char *rom = "/home/stuart/mario.gb";
		#elif 0
			const char *rom = "/home/stuart/drmario.gb";
		#elif 0
			const char *rom = "/home/stuart/pokemon_blue.gb";
		#elif 0
			const char *rom = "/home/stuart/cpu_instrs.gb";
		#elif 1
			const char *rom = "/home/stuart/Documents/Gameboy/cpu_instrs/individual/01-special.gb";
		#else
			const char *rom = "/home/stuart/Documents/Gameboy/cpu_instrs/individual/05-op rp.gb";
		#endif
		struct cpu_state *state = cpu_init("./data/boot_roms/DMG.bin", rom);
		cpu_start(state);
		cpu_delete(state);
	#endif
	return 0;
}
