#include <stdio.h>
#include <string.h>

#include "rom.h"
#include "cpu.h"
#include "display.h"
#include "ppm.h"
#include "testing/testing.h"
#include "audio.h"
#include "cmdline.h"

#include <SDL2/SDL.h>
int main(int argc, char **argv)
{
	cmdline_t cmdline;
	cmdline = cmdline_read(argc, argv);
	if(TESTING)
	{
		testing(argc, argv);
	}
	else
	{
		struct cpu_state *state = cpu_init(&cmdline);
		cpu_start(state);
		cpu_delete(state);
	}
	return 0;
}
