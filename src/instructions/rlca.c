#include "cpu.h"

void RLCA(cpu_state_t *state,
		enum ARG_TYPE arg0, union REG_INPUT i0,
		enum ARG_TYPE arg1, union REG_INPUT i1)
{
	cpu_rlca(state);
}

void cpu_rlca(cpu_state_t *state)
{
	state->a = cpu_rlc(state, state->a);
    cpu_set_zero(state, 0);
}
