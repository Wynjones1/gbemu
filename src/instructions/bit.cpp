#include "cpu.h"

#define X(n) case ARG_TYPE_ ## n: cpu_bit(state, n, d0); break;
void BIT(struct cpu_state *state,
		enum ARG_TYPE arg0, union REG_INPUT i0,
		enum ARG_TYPE arg1, union REG_INPUT i1)
{
	reg_t d0;
	if(arg1 == ARG_TYPE_REG16_INDIRECT)
	{
		d0 = cpu_load8_indirect(state, i1);
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
	}
	else if(arg1 == ARG_TYPE_REG8)
	{
		d0 = cpu_load_reg8(state, i1);
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
				Error("Invalid argument type in bit command");
				break;
		}
	}
}
#undef X

void cpu_bit(struct cpu_state *state, reg_t pos, reg_t d0)
{
	state->zero = !((d0 >> pos) & 0x1);
	state->subtract = 0;
	state->half_carry = 1;
}
