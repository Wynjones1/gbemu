#include "cpu.h"

void RLA(struct cpu_state *state,
		enum ARG_TYPE arg0, REG_INPUT i0,
		enum ARG_TYPE arg1, REG_INPUT i1)
{
	cpu_rla(state);
}

//TODO:Check if these actually set the zero flag.
void cpu_rla(struct cpu_state *state)
{
	state->a = cpu_rl(state, state->a);
}
