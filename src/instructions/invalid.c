#include "cpu.h"

void INVALID(cpu_state_t *state,
		enum ARG_TYPE arg0, union REG_INPUT i0,
		enum ARG_TYPE arg1, union REG_INPUT i1)
{
	if(arg0 == ARG_TYPE_NONE)
	{
		if(arg1 == ARG_TYPE_NONE)
		{
		}
	}
}