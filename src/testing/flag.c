#include "testing.h"

int flag_test(void)
{
	cpu_state_t state;
    memset(&state, 0x00, sizeof(state));
	state.f          = 0;
	cpu_set_carry(&state, 1);
	cpu_set_half_carry(&state, 1);
	cpu_set_subtract(&state, 1);
	printf("%02x\n", state.f);
	return 0;
}
