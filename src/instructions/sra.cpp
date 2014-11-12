#include "cpu.h"

void SRA(struct cpu_state *state,
		enum ARG_TYPE arg0, REG_INPUT i0,
		enum ARG_TYPE arg1, REG_INPUT i1)
{
	if(arg0 == ARG_TYPE_REG16_INDIRECT)
	{
		reg_t d0 = cpu_load8_indirect(state, i0);
		d0 = cpu_sra(state, d0);
		cpu_store8_indirect(state, i0, d0);
	}
	else if(arg0 == ARG_TYPE_REG8)
	{
		reg_t d0 = cpu_load_reg8(state, i0);
		d0 = cpu_sra(state, d0);
		cpu_store_reg8(state, i0, d0);
	}
}

reg_t   cpu_sra(struct cpu_state *state, reg_t d0)
{
	state->carry = d0 & 0x1;
	d0 = (d0 >> 1) | ((d0 >> 7) << 7);
	state->zero       = (d0 == 0);
	state->half_carry = 0;
	state->subtract   = 0;
	return d0;
}
