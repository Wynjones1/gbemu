#include "cpu.h"

void CCF(cpu_state_t *state,
		enum ARG_TYPE arg0, union REG_INPUT i0,
		enum ARG_TYPE arg1, union REG_INPUT i1)
{
	cpu_set_carry(state, !cpu_carry(state));
	cpu_set_subtract(state, 0);
	cpu_set_half_carry(state, 0);
}
