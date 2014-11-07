#include "cpu.h"

void POP(struct cpu_state *state,
		ARG_TYPE arg0, REG_INPUT i0,
		ARG_TYPE arg1, REG_INPUT i1)
{
	cpu_pop(state, i0); 
}

void cpu_pop(struct cpu_state *state, REG_INPUT reg)
{
	state->registers16[reg.r16]  = cpu_load16(state, state->sp);
	state->sp += 2;
}
