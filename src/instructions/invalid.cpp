#include "cpu.h"

void INVALID(struct cpu_state *state,
		enum ARG_TYPE arg0, REG_INPUT i0,
		enum ARG_TYPE arg1, REG_INPUT i1)
{
	if(arg0 == ARG_TYPE_NONE)
	{
		if(arg1 == ARG_TYPE_NONE)
		{
		}
	}
}