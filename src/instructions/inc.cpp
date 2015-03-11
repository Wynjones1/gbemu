#include "cpu.h"

void INC(struct cpu_state *state,
		const enum ARG_TYPE arg0, const union REG_INPUT i0,
		const enum ARG_TYPE arg1, const union REG_INPUT i1)
{
	if(arg0 == ARG_TYPE_REG16)
	{
		cpu_inc16(state, i0);
	}
	else if(arg0 == ARG_TYPE_REG16_INDIRECT)
	{
		reg_t res                = cpu_load8_indirect(state, i0);
		cpu_set_half_carry(state, (res == 0xf));
		res += 1;
		cpu_set_zero(state, (res == 0));
		cpu_set_subtract(state, 0);
		cpu_store8_indirect(state, i0, res);
	}
	else if(arg0 == ARG_TYPE_REG8)
	{
		reg_t data = cpu_load_reg8(state, i0);
		cpu_store_reg8(state, i0, cpu_inc8(state, data));
	}
}

reg_t cpu_inc8(struct cpu_state *state, reg_t d0)
{
	int old_carry = cpu_carry(state);
	reg_t res     = cpu_add8(state, d0, 1);
	cpu_set_carry(state, old_carry);
	return res;
}

void cpu_inc16(struct cpu_state *state, REG_INPUT reg)
{
	//No flags are affected with the 16bit increment.
	state->registers16[reg.r16] += 1;
}
