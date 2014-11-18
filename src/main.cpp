#include <stdio.h>
#include <string.h>

#include "rom.h"
#include "cpu.h"
#include "display.h"
#include "ppm.h"
#include "testing/testing.h"
#include "audio.h"
#include "cmdline.h"
#include "debug.h"

#include <SDL2/SDL.h>
int main(int argc, char **argv)
{
	cmdline_t cmdline = cmdline_read(argc, argv);
	if(TESTING)
	{
		testing(argc, argv);
	}
	else
	{
		struct cpu_state *state = cmdline.state_file ?
			cpu_load_state("game.state")          :
			cpu_init(cmdline.boot_rom, cmdline.in);

		state->cmdline = cmdline;
		cpu_start(state);
		if(DISPLAY_THREAD)
		{
			display_join(state->display);
		}
		debug_on_exit(state);
		cpu_delete(state);
	}
	return 0;
}
