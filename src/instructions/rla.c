#include "cpu.h"

void RLA(cpu_state_t *state,
		enum ARG_TYPE arg0, union REG_INPUT i0,
		enum ARG_TYPE arg1, union REG_INPUT i1)
{
	cpu_rla(state);
}

void cpu_rla(cpu_state_t *state)
{
	state->a = cpu_rl(state, state->a);
    cpu_set_zero(state, 0);
}
