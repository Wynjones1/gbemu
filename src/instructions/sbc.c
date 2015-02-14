#include "cpu.h"

void SBC(struct cpu_state *state,
		enum ARG_TYPE arg0, union REG_INPUT i0,
		enum ARG_TYPE arg1, union REG_INPUT i1)
{
	reg_t data;
	if(arg1 == ARG_TYPE_DATA8)
	{
		data = state->arg;
	}
	else if(arg1 == ARG_TYPE_REG16_INDIRECT)
	{
		data = cpu_load8_indirect(state, i1);
	}
	else if(arg1 == ARG_TYPE_REG8)
	{
		data = cpu_load_reg8(state, i1);
	}
	else
	{
		Error("Invalid argument type.\n");
	}
	cpu_sbc(state, data);
}

void cpu_sbc(struct cpu_state *state, reg_t d0)
{
	reg16_t t0        = d0 + state->carry;
	cpu_set_subtract(state, 1);
	cpu_set_carry(state, state->a < t0);
	cpu_set_half_carry(state, (state->a & 0xf) < (t0 & 0xf));
	state->a          = (state->a - (reg_t)t0);
	cpu_set_zero(state, (state->a == 0));
}
