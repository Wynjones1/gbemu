#include "cpu.h"

void AND(cpu_state_t *state,
		const enum ARG_TYPE arg0, const union REG_INPUT i0,
		const enum ARG_TYPE arg1, const union REG_INPUT i1)
{
	reg_t data0 = cpu_load_reg8(state, A_REG);
	reg_t data1;
	if(arg0 == ARG_TYPE_DATA8)
	{
		data1 = (reg_t)state->arg;
	}
	else if(arg0 == ARG_TYPE_REG16_INDIRECT)
	{
		data1 = (reg_t)cpu_load16_indirect(state, i0);
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

reg_t cpu_and(cpu_state_t *state, reg_t d0, reg_t d1)
{
	reg_t res         = d0 & d1;
	cpu_set_zero(state, res == 0);
	cpu_set_half_carry(state, 1);
	cpu_set_carry(state, 0);
	cpu_set_subtract(state, 0);
	return res;
}
