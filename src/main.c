#include "cmdline.h"
#include "cpu.h"
#include "debug.h"

int main(int argc, char **argv)
{
	cmdline_read(argc, argv);
    debug_init();
    cpu_state_t *state = cpu_init();
    cpu_start(state);
    cpu_delete(state);
	return 0;
}
