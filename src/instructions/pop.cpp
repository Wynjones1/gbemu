#include "cpu.h"

void POP(struct cpu_state *state,
		const enum ARG_TYPE arg0, const union REG_INPUT i0,
		const enum ARG_TYPE arg1, const union REG_INPUT i1)
{
	cpu_pop(state, i0); 
}

void cpu_pop(struct cpu_state *state, REG_INPUT reg)
{
	state->registers16[reg.r16]  = cpu_load16(state, state->sp);
	state->sp += 2;
}
