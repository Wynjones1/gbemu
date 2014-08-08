#include "cpu.h"

void RETI(struct cpu_state *state,
		enum ARG_TYPE arg0, union REG_INPUT i0,
		enum ARG_TYPE arg1, union REG_INPUT i1)
{
	cpu_reti(state);
}

void cpu_reti(struct cpu_state *state)
{
	REG_INPUT t0 = {.r16 = REG16_SP};
	state->jump  = 1;
	cpu_pop(state, t0);
	cpu_enable_interrupts(state);
}
