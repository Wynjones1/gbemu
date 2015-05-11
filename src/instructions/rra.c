#include "cpu.h"

void RRA(cpu_state_t *state,
		enum ARG_TYPE arg0, union REG_INPUT i0,
		enum ARG_TYPE arg1, union REG_INPUT i1)
{
	cpu_rra(state);
}

void cpu_rra(cpu_state_t *state)
{
	state->a = cpu_rr(state, state->a);
    cpu_set_zero(state, 0);
}
