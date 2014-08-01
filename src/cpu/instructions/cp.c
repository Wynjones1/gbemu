#include "cpu.h"

void CP(struct cpu_state *state,
		const enum ARG_TYPE arg0, const union REG_INPUT i0,
		const enum ARG_TYPE arg1, const union REG_INPUT i1)
{
	reg_t d0;
	if(arg0 == ARG_TYPE_DATA8)
	{
		d0 = state->memory[state->pc + 1];
	}
	else if(arg0 == ARG_TYPE_REG16_INDIRECT)
	{
		d0 = cpu_load8_indirect(state, i0);
	}
	else if(arg0 == ARG_TYPE_REG8)
	{
		d0 = cpu_load_reg8(state, i0);
	}
	cpu_cmp(state, d0);
}
