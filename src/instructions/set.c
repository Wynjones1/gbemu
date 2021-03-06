#include "cpu.h"

#define X(n) case ARG_TYPE_ ## n: d0 = cpu_set(state, n, d0); break;
void SET(cpu_state_t *state,
		enum ARG_TYPE arg0, union REG_INPUT i0,
		enum ARG_TYPE arg1, union REG_INPUT i1)
{
	if(arg1 == ARG_TYPE_REG16_INDIRECT)
	{
		reg_t d0 = cpu_load8_indirect(state, i1);
		switch(arg0)
		{
			X(0);
			X(1);
			X(2);
			X(3);
			X(4);
			X(5);
			X(6);
			X(7);
			default:
				Error("Invalid argument type");
				break;
		}
		cpu_store8_indirect(state, i1, d0);
	}
	else if(arg1 == ARG_TYPE_REG8)
	{
		reg_t d0 = cpu_load_reg8(state, i1);
		switch(arg0)
		{
			X(0);
			X(1);
			X(2);
			X(3);
			X(4);
			X(5);
			X(6);
			X(7);
			default:
				Error("Invalid argument type");
				break;
		}
		cpu_store_reg8(state, i1, d0);
	}
}
#undef X

reg_t cpu_set(cpu_state_t *state, reg_t pos, reg_t d0)
{
	return d0 | 1 << pos;
}
