#include "cpu.h"
#include <stdio.h>

void SUB(struct cpu_state *state,
		enum ARG_TYPE arg0, union REG_INPUT i0,
		enum ARG_TYPE arg1, union REG_INPUT i1)
{
	reg_t data;
	if(arg0 == ARG_TYPE_DATA8)
	{
		data = cpu_load8(state, state->pc + 1);
	}
	else if(arg0 == ARG_TYPE_REG16_INDIRECT)
	{
		data = cpu_load8_indirect(state, i0);
	}
	else if(arg0 == ARG_TYPE_REG8)
	{
		data = cpu_load_reg8(state, i0);
	}
	cpu_sub(state, data);
}

void cpu_sub(struct cpu_state *state, reg_t d0)
{
	int16_t res = state->a - d0;
	state->carry = 0;
	if(res < 0)
	{
		res += 0xff;
		state->carry = 1;
	}
	state->a        = res;
	state->zero     = (state->a == 0);
	state->subtract = 1;
}
