#include "cpu.h"

void EI(struct cpu_state *state,
		enum ARG_TYPE arg0, REG_INPUT i0,
		enum ARG_TYPE arg1, REG_INPUT i1)
{
	cpu_enable_interrupts(state);
}

void cpu_enable_interrupts(struct cpu_state *state)
{
	state->EI_Pending = 1;
}
