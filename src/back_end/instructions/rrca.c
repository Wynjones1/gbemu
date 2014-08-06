#include "cpu.h"

void RRCA(struct cpu_state *state,
		enum ARG_TYPE arg0, union REG_INPUT i0,
		enum ARG_TYPE arg1, union REG_INPUT i1)
{
	cpu_rrca(state);
}

/* Rotate A right, store bit 0 in carry flag */
void cpu_rrca(struct cpu_state *state)
{
	state->a = cpu_rrc(state, state->a);
}
