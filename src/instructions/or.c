#include "cpu.h"

void OR(cpu_state_t *state,
		const enum ARG_TYPE arg0, const union REG_INPUT i0,
		const enum ARG_TYPE arg1, const union REG_INPUT i1)
{
	reg_t d0, d1;
	if(arg0 == ARG_TYPE_DATA8)
	{
		d1 = (reg_t)state->arg;
	}
	else if(arg0 == ARG_TYPE_REG16_INDIRECT)
	{
		d1 = cpu_load8_indirect(state, i0);
	}
	else if(arg0 == ARG_TYPE_REG8)
	{
		d1 = cpu_load_reg8(state, i0);
	}
	else
	{
		Error("Invalid argument type.\n");
	}
	d0 = cpu_load_reg8(state, A_REG);
	d0 = cpu_or(state, d0, d1);
	cpu_store_reg8(state, A_REG, d0);
}

reg_t cpu_or(cpu_state_t *state, reg_t d0, reg_t d1)
{
	reg_t res         = d0 | d1;
	cpu_set_zero(state, res == 0);
	cpu_set_half_carry(state, 0);
	cpu_set_carry(state, 0);
	cpu_set_subtract(state, 0);
	return res;
}
