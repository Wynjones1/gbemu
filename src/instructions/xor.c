#include "cpu.h"

void XOR(struct cpu_state *state,
		enum ARG_TYPE arg0, union REG_INPUT i0,
		enum ARG_TYPE arg1, union REG_INPUT i1)
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
	cpu_xor(state, data);
}

void cpu_xor(struct cpu_state *state, reg_t d0)
{
	state->a ^= d0;
	state->zero       = (state->a == 0);
	state->half_carry = 0;
	state->carry      = 0;
	state->subtract   = 0;
}
