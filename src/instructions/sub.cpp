#include "cpu.h"
#include <stdio.h>

void SUB(struct cpu_state *state,
		enum ARG_TYPE arg0, REG_INPUT i0,
		enum ARG_TYPE arg1, REG_INPUT i1)
{
	reg_t data;
	if(arg0 == ARG_TYPE_DATA8)
	{
		data = state->arg;
	}
	else if(arg0 == ARG_TYPE_REG16_INDIRECT)
	{
		data = cpu_load8_indirect(state, i0);
	}
	else if(arg0 == ARG_TYPE_REG8)
	{
		data = cpu_load_reg8(state, i0);
	}
	else
	{
		Error("Invalid argument type.\n");
	}
	cpu_sub(state, data);
}

void cpu_sub(struct cpu_state *state, reg_t d0)
{
	int16_t res = state->a - d0;
	state->carry = 0;
	if(res < 0)
	{
		res += 0x100;
		state->carry = 1;
	}
	state->half_carry = (state->a & 0xf) < (d0 & 0xf);
	state->a          = res;
	state->zero       = (state->a == 0);
	state->subtract   = 1;
}
