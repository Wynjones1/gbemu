#include "cpu.h"

#define X(n) case ARG_TYPE_ ## n: cpu_bit(state, n, d0); break;
void BIT(cpu_state_t *state,
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

void cpu_bit(cpu_state_t *state, reg_t pos, reg_t d0)
{
	cpu_set_zero(state, !((d0 >> pos) & 0x1));
	cpu_set_subtract(state, 0);
	cpu_set_half_carry(state, 1);
}
