#include "cpu.h"

void PUSH(struct cpu_state *state,
		const enum ARG_TYPE arg0, const union REG_INPUT i0,
		const enum ARG_TYPE arg1, const union REG_INPUT i1)
{
	if(arg0 == ARG_TYPE_REG16)
	{
		if(arg1 == ARG_TYPE_NONE)
		{
		}
	}
}