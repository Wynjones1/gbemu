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
		state->half_carry        = (res == 0xf);
		res += 1;
		state->zero              = (res == 0);
		state->subtract          = 0;
		cpu_store8_indirect(state, i0, res);
	}
	else if(arg0 == ARG_TYPE_REG8)
	{
		cpu_inc8(state, i0);
	}
}
