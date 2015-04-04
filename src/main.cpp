#include <stdio.h>
#include <string.h>

#include "common.h"
#include "audio.h"
#include "cmdline.h"
#include "cpu.h"
#include "debug.h"
#include "display.h"
#include "ppm.h"
#include "rom.h"

#if TESTING
#include "testing/testing.h"
#endif

int main(int argc, char **argv)
{
	cmdline_read(argc, argv);
    debug_init();
#if TESTING
    testing(argc, argv);
#else
    struct cpu_state *state = cpu_init();
    cpu_start(state);
    cpu_delete(state);
#endif
	return 0;
}