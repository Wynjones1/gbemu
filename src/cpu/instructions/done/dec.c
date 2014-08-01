#include "cpu.h"

void DEC(struct cpu_state *state,
		const enum ARG_TYPE arg0, const union REG_INPUT i0,
		const enum ARG_TYPE arg1, const union REG_INPUT i1)
{
	if(arg0 == ARG_TYPE_REG16)
	{
		cpu_dec16(state, i0);
	}
	else if(arg0 == ARG_TYPE_REG16_INDIRECT)
	{
		//TODO: immediate decrement.
	}
	else if(arg0 == ARG_TYPE_REG8)
	{
		cpu_dec8(state, i0);
	}
}
