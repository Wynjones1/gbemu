#include <stdio.h>
#include <string.h>

#include "rom.h"
#include "cpu.h"
#include "display.h"

#include <SDL2/SDL.h>

int main(int argc, char **argv)
{
#if 1
	struct cpu_state *state = cpu_init("./data/boot_roms/DMG.bin");
	#if 0
		cpu_load_rom(state, "/home/stuart/tetris.gb");
	#elif 1
		cpu_load_rom(state, "/home/stuart/mario.gb");
	#else
		cpu_load_rom(state, "/home/stuart/pokemon_blue.gb");
	#endif
	cpu_start(state);
	cpu_delete(state);
#else
	display_t *dis = display_init();
	char rgb[3] = {0xff, 0xff, 0xff};
	for(int i = 0; i < 10; i++)
	{
		display_draw_pixel(dis, i, i, rgb);
		SDL_Delay(500);
	}
	display_present(dis);
#endif
	return 0;
}
