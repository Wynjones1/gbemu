#include "cpu.h"
#include <stdio.h>

void SUB(struct cpu_state *state,
		enum ARG_TYPE arg0, union REG_INPUT i0,
		enum ARG_TYPE arg1, union REG_INPUT i1)
{
	reg_t data;
	if(arg0 == ARG_TYPE_DATA8)
	{
		data = (reg_t)state->arg;
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
	cpu_set_carry(state, 0);
	if(res < 0)
	{
		res += 0x100;
		cpu_set_carry(state, 1);
	}
	cpu_set_half_carry(state, (state->a & 0xf) < (d0 & 0xf));
	state->a          = res & 0xff;
	cpu_set_zero(state, (state->a == 0));
	cpu_set_subtract(state, 1);
}
