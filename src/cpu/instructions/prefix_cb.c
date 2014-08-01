#include "cpu.h"

void PREFIX_CB(struct cpu_state *state,
		const enum ARG_TYPE arg0, const union REG_INPUT i0,
		const enum ARG_TYPE arg1, const union REG_INPUT i1)
{
	if(arg0 == ARG_TYPE_NONE)
	{
		if(arg1 == ARG_TYPE_NONE)
		{
		}
	}
}