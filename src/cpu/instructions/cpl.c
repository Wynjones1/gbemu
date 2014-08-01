#include "cpu.h"

void CPL(struct cpu_state *state,
		enum ARG_TYPE arg0, union REG_INPUT i0,
		enum ARG_TYPE arg1, union REG_INPUT i1)
{
	state->half_carry = 1;
	state->subtract   = 1;
	state->a = ~state->a;
}
