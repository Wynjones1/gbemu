#include "cpu.h"

void EI(cpu_state_t *state,
		enum ARG_TYPE arg0, union REG_INPUT i0,
		enum ARG_TYPE arg1, union REG_INPUT i1)
{
	cpu_enable_interrupts(state);
}

void cpu_enable_interrupts(cpu_state_t *state)
{
	state->EI_Pending = 1;
}
