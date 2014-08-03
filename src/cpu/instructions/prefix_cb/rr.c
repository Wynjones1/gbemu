#include "cpu.h"

void RR(struct cpu_state *state,
		enum ARG_TYPE arg0, union REG_INPUT i0,
		enum ARG_TYPE arg1, union REG_INPUT i1)
{
	if(arg0 == ARG_TYPE_REG16_INDIRECT)
	{
		if(arg1 == ARG_TYPE_NONE)
		{
		}
	}
	else if(arg0 == ARG_TYPE_REG8)
	{
		if(arg1 == ARG_TYPE_NONE)
		{
		}
	}
}