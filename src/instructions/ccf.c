#include "cpu.h"

void CCF(struct cpu_state *state,
		enum ARG_TYPE arg0, union REG_INPUT i0,
		enum ARG_TYPE arg1, union REG_INPUT i1)
{
	cpu_set_carry(state, !state->carry);
	cpu_set_subtract(state, 0);
	cpu_set_half_carry(state, 0);
}
