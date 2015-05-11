#include "cpu.h"

void DI(cpu_state_t *state,
		enum ARG_TYPE arg0, union REG_INPUT i0,
		enum ARG_TYPE arg1, union REG_INPUT i1)
{
	cpu_disable_interrupts(state);
}

void cpu_disable_interrupts(cpu_state_t *state)
{
	state->DI_Pending = 1;
}
