#include <stdio.h>
#include <string.h>

#include "rom.h"
#include "cpu.h"
#include "display.h"

#include <SDL2/SDL.h>

int main(int argc, char **argv)
{
#if 1
	struct cpu_state state;
	memset(&state, 0x00, sizeof(struct cpu_state));
	for(int i = 0; i < 0xff; i++)
	{
		struct opcode code = op_table[i];
		code.op(&state, code.arg0, code.i0, code.arg1, code.i1);
	}
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
