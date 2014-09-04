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
			const char *rom = "./data/roms/mario.gb";
		#elif 1
			const char *rom = "./data/roms/zelda.gb";
		#elif 1
			const char *rom = "./data/roms/Donkey Kong Land.gb";
		#elif 1
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
		#if 1
		struct cpu_state *state = cpu_init("./data/boot_roms/DMG.bin", rom);
		#else
		struct cpu_state *state = cpu_load_state("game.state");
		cpu_save_state(state, "out.state");
		exit(0);
		#endif
	
		cpu_start(state);
		cpu_delete(state);
	#endif
	return 0;
}
