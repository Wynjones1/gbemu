#include "cpu.h"

void CP(cpu_state_t *state,
		const enum ARG_TYPE arg0, const union REG_INPUT i0,
		const enum ARG_TYPE arg1, const union REG_INPUT i1)
{
	reg_t d0;
	if(arg0 == ARG_TYPE_DATA8)
	{
		d0 = (reg_t)state->arg;
	}
	else if(arg0 == ARG_TYPE_REG16_INDIRECT)
	{
		d0 = (reg_t)cpu_load8_indirect(state, i0);
	}
	else if(arg0 == ARG_TYPE_REG8)
	{
		d0 = (reg_t)cpu_load_reg8(state, i0);
	}
	else
	{
		Error("Incorrect argument type.\n");
	}
	cpu_cmp(state, d0);
}

void cpu_cmp(cpu_state_t *state, reg_t d0)
{
	cpu_set_carry(state, state->a < d0);
	cpu_set_zero(state, state->a == d0);
	cpu_set_half_carry(state, (state->a & 0xf) < (d0 & 0xf));
	cpu_set_subtract(state, 1);
}
