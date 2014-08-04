#include "cpu.h"

void AND(struct cpu_state *state,
		const enum ARG_TYPE arg0, const union REG_INPUT i0,
		const enum ARG_TYPE arg1, const union REG_INPUT i1)
{
	reg_t data0 = cpu_load_reg8(state, A_REG);
	reg_t data1;
	if(arg0 == ARG_TYPE_DATA8)
	{
		data1 = cpu_load8(state, state->pc + 1);
	}
	else if(arg0 == ARG_TYPE_REG16_INDIRECT)
	{
		data1 = cpu_load16_indirect(state, i0);
	}
	else if(arg0 == ARG_TYPE_REG8)
	{
		data1 = cpu_load_reg8(state, i0);
	}
	data0 = cpu_and(state, data0, data1);
	cpu_store_reg8(state, A_REG, data0);
}
