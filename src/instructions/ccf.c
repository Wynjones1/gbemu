#include "cpu.h"

void CCF(struct cpu_state *state,
		enum ARG_TYPE arg0, union REG_INPUT i0,
		enum ARG_TYPE arg1, union REG_INPUT i1)
{
	state->carry = !state->carry;
	state->subtract   = 0;
	state->half_carry = 0;
}
