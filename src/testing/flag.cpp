#include "testing.h"
#include <string.h>

int flag_test(void)
{
	cpu_state_t state;
	memset(&state, 0x00, sizeof(state));
	state.f = 0;
	state.carry = 1;
	state.half_carry = 1;
	state.subtract = 1;
	printf("%02x\n", state.f);
	return 0;
}
