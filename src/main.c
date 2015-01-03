#include <stdio.h>
#include <string.h>
#include <SDL2/SDL.h>

#include "audio.h"
#include "cmdline.h"
#include "cpu.h"
#include "debug.h"
#include "display.h"
#include "ppm.h"
#include "rom.h"
#include "testing/testing.h"

int main(int argc, char **argv)
{
	cmdline_read(argc, argv);
    debug_init();
	if(TESTING)
	{
		testing(argc, argv);
	}
	else
	{
		struct cpu_state *state = cpu_init();
		cpu_start(state);
		cpu_delete(state);
	}
	return 0;
}
