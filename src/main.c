#include <stdio.h>
#include <string.h>

#include "rom.h"
#include "cpu.h"
#include "display.h"

#include <SDL2/SDL.h>

int main(int argc, char **argv)
{
	#if 0
		const char *rom = "/home/stuart/tetris.gb";
	#elif 1
		const char *rom = "/home/stuart/mario.gb";
	#else
		const char *rom = "/home/stuart/pokemon_blue.gb";
	#endif
	struct cpu_state *state = cpu_init("./data/boot_roms/DMG.bin", rom);
	cpu_start(state);
	cpu_delete(state);
	return 0;
}
