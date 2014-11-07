#include "cpu.h"

void AND(struct cpu_state *state,
		ARG_TYPE arg0, REG_INPUT i0,
		ARG_TYPE arg1, REG_INPUT i1)
{
	reg_t data0 = cpu_load_reg8(state, A_REG);
	reg_t data1;
	if(arg0 == ARG_TYPE_DATA8)
	{
		data1 = state->arg;
	}
	else if(arg0 == ARG_TYPE_REG16_INDIRECT)
	{
		data1 = cpu_load16_indirect(state, i0);
	}
	else if(arg0 == ARG_TYPE_REG8)
	{
		data1 = cpu_load_reg8(state, i0);
	}
	else
	{
		Error("Incorrect argument type.\n");
	}
	data0 = cpu_and(state, data0, data1);
	cpu_store_reg8(state, A_REG, data0);
}

reg_t cpu_and(struct cpu_state *state, reg_t d0, reg_t d1)
{
	reg_t res         = d0 & d1;
	state->zero       = res == 0;
	state->half_carry = 1;
	state->carry      = 0;
	state->subtract   = 0;
	return res;
}
