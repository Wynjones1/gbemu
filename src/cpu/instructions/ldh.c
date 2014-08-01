#include "cpu.h"

void LDH(struct cpu_state *state,
		const enum ARG_TYPE arg0, const union REG_INPUT i0,
		const enum ARG_TYPE arg1, const union REG_INPUT i1)
{
	if(arg0 == ARG_TYPE_DATA16_UNSIGNED_INDIRECT)
	{
		if(arg1 == ARG_TYPE_REG8)
		{
		}
	}
	else if(arg0 == ARG_TYPE_REG8)
	{
		if(arg1 == ARG_TYPE_DATA16_UNSIGNED_INDIRECT)
		{
		}
	}
}
