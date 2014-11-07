#include "cpu.h"

void CP(struct cpu_state *state,
		ARG_TYPE arg0, REG_INPUT i0,
		ARG_TYPE arg1, REG_INPUT i1)
{
	reg_t d0;
	if(arg0 == ARG_TYPE_DATA8)
	{
		d0 = state->arg;
	}
	else if(arg0 == ARG_TYPE_REG16_INDIRECT)
	{
		d0 = cpu_load8_indirect(state, i0);
	}
	else if(arg0 == ARG_TYPE_REG8)
	{
		d0 = cpu_load_reg8(state, i0);
	}
	else
	{
		Error("Incorrect argument type.\n");
	}
	cpu_cmp(state, d0);
}

void cpu_cmp(struct cpu_state *state, reg_t d0)
{
	state->carry      = state->a < d0;
	state->zero       = state->a == d0;
	state->half_carry = (state->a & 0xf) < (d0 & 0xf);
	state->subtract   = 1;
}
