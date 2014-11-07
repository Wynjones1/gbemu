#include "cpu.h"

void DEC(struct cpu_state *state,
		ARG_TYPE arg0, REG_INPUT i0,
		ARG_TYPE arg1, REG_INPUT i1)
{
	if(arg0 == ARG_TYPE_REG16)
	{
		cpu_dec16(state, i0);
	}
	else if(arg0 == ARG_TYPE_REG16_INDIRECT)
	{
		reg_t res                = cpu_load8_indirect(state, i0);
		state->half_carry        = (res & 0xf) != 0;
		res -= 1;
		state->zero              = (res == 0);
		state->subtract          = 1;
		cpu_store8_indirect(state, i0, res);
	}
	else if(arg0 == ARG_TYPE_REG8)
	{
		reg_t d0 = cpu_load_reg8(state, i0);
		state->registers[i0.r8] = cpu_dec8(state, d0);
	}
}

reg_t cpu_dec8(struct cpu_state *state, reg_t d0)
{
	reg_t res                = d0 - 1;
	state->zero              = (res == 0);
	state->subtract          = 1;
	state->half_carry        = (d0 & 0xf) == 0;
	return res;
}

void cpu_dec16(struct cpu_state *state, REG_INPUT reg)
{
	//No flags are affected with the 16bit decrement.
	state->registers16[reg.r16] -= 1;
}
