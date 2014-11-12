#include "cpu.h"

void DI(struct cpu_state *state,
		enum ARG_TYPE arg0, REG_INPUT i0,
		enum ARG_TYPE arg1, REG_INPUT i1)
{
	cpu_disable_interrupts(state);
}

void cpu_disable_interrupts(struct cpu_state *state)
{
	state->DI_Pending = 1;
}
