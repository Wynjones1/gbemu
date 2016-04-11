#include "cmdline.h"
#include "cpu.h"
#include "debug.h"

#ifndef ROM_FILE
	#error "ROM_FILE must be defined."
#endif
int main(int argc, char **argv)
{
	cmdline_read(argc, argv);
	cmdline_args.in = ROM_FILE;
	debug_init();
	cpu_state_t *state = cpu_init();
	cpu_start(state);
	cpu_delete(state);
	return 0;
}
