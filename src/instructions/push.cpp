#include "cpu.h"

void PUSH(struct cpu_state *state,
		const enum ARG_TYPE arg0, const union REG_INPUT i0,
		const enum ARG_TYPE arg1, const union REG_INPUT i1)
{
	reg16_t d = cpu_load_reg16(state, i0);
	cpu_push(state, d);
}

void  cpu_push(struct cpu_state *state, reg16_t d0)
{
	state->sp -= 2;
	cpu_store16(state, state->sp, d0);
}